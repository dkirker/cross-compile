NAME = sdlvnc
VERSION = 0.1

SRC_GIT = git://git.webos-internals.org/applications/${NAME}.git

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -z -x -f $<
	touch $@

include ../../support/staging.mk

stage-local::

include ../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure )
	( cd build/src ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
