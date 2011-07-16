NAME = cairo
VERSION = 1.8.10

DEPENDS = media/libpng12 \
		  x/fontconfig \
		  x/pixman \
		  x/libx11

SRC_TGZ = http://cairographics.org/releases/${NAME}-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
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
	LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
	)	
	( cd build/$* ; ${SB2} $(MAKE))
	( cd build/$* ; ${SB2} $(MAKE) install)
	touch $@

clobber::
	rm -rf build
