NAME = libxcb
VERSION = 1.7

DEPENDS = x/xcbproto         \
		  x/libpthread-stubs \
		  x/xau

SRC_BZ2 = http://xcb.freedesktop.org/dist/${NAME}-${VERSION}.tar.bz2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	sed -i s/src\ tests\ doc/src\ tests/g build/src/Makefile.in
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built-${VERSION}

build/%.built-${VERSION}: build/.unpacked-${VERSION}
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} autoreconf -i )
	( cd build/$* ; ${SB2} \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--disable-build-docs \
	)
	( cd build/$* ; ${SB2} $(MAKE))
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
