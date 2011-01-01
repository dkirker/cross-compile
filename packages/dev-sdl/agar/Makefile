NAME = agar
VERSION = 1.4.0
DEPENDS = dev-sdl/libsdl

SRC_TGZ = http://stable.hypertriton.com/agar/${NAME}-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	( cd build/src ; patch -p0 < ../../sdlinit.patch )
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} CC="gcc" ./configure --with-jpeg=no --with-png=no --with-sdl=yes )
	( cd build/$* ; ${SB2} CC="gcc" $(MAKE) )
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
