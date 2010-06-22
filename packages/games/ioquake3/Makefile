NAME = ioquake3
VERSION = 1.36

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

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src/src ; ${SB2} $(MAKE) )
	touch $@

clobber::
	rm -rf build
