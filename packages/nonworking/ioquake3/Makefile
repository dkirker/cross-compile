NAME = ioquake3
VERSION = 1.36

DEPENDS = dev-sdl/libsdl

SRC_GIT = git://git.webos-internals.org/games/ioquake3.git

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -xf ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$*/src ; ${SB2} $(MAKE) )
	touch $@

clobber::
	rm -rf build
