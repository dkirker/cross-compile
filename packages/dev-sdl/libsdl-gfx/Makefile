NAME = SDL_gfx
VERSION = 2.0.20
DEPENDS = dev-sdl/libsdl

SRC_TGZ = http://www.ferzkopp.net/Software/SDL_gfx-2.0/${NAME}-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -f $@
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} PATH=/usr/local/bin:${PATH} ./configure --disable-mmx )
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
