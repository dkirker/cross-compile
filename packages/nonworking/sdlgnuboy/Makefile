NAME = sdlgnuboy
VERSION = 0.0.3

SRC_GIT = git://git.webos-internals.org/games/${NAME}.git

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	touch $@

include ../../support/staging.mk

stage::

include ../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure ac_cv_header_linux_joystick_h=no )
	( cd build/src ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
