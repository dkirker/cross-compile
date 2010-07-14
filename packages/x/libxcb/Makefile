NAME = libxcb
VERSION = 1.5

DEPENDS = x/xcbproto         \
		  x/libpthread-stubs \
		  x/xau

SRC_BZ2 = http://xcb.freedesktop.org/dist/${NAME}-${VERSION}.tar.bz2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	sed -i s/src\ tests\ doc/src\ tests/g build/src/Makefile.in
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
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
