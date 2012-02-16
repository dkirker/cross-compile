NAME = fontconfig
VERSION = 2.8.0

DEPENDS = common/libxml2 common/freetype

SRC_BZ2 = http://cgit.freedesktop.org/fontconfig/snapshot/${NAME}-${VERSION}.tar.bz2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built-${VERSION}

build/%.built-${VERSION}: build/.unpacked-${VERSION}
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} \
	./autogen.sh \
		--prefix=${INSTALL_PREFIX} --sysconfdir=/usr/local/share \
		--host=i386 --build=arm --with-arch=arm --disable-docs \
	)	
	#XXX the libtool which ships with fontconfig generates wierd errors
	# Using the one from scratchbox works fine right now
	( cd build/$* ; ${SB2} cp \`which libtool\` .)
	( cd build/$* ; ${SB2} $(MAKE) )
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
