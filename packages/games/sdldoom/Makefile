NAME = sdldoom
VERSION = 1.10

SRC_TGZ = http://www.libsdl.org/projects/doom/src/sdldoom-1.10.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage-local::

include ../../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --with-sdl-prefix=/usr/local )
	( cd build/src ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
