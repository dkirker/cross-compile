NAME = rdesktop-sdl
VERSION = 1.1.0
DEPENDS = libsdl

SRC_TGZ = http://www.soulsphere.org/hacks/rdesktop-sdl/rdesktop-1.1.0-sdl.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/rdesktop-${VERSION}-sdl build/src
	sed -i.orig -e 's|-I/usr/X11R6/include ||' -e 's|-L/usr/X11R6/lib -lX11 ||' \
		build/src/Makefile
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} $(MAKE) CC=gcc )
	touch $@

clobber::
	rm -rf build
