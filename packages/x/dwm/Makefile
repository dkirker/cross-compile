NAME = dwm
VERSION = 5.9

# TODO: Actual deps?
# These are just stolen from xterm.
DEPENDS = \
		  x/libx11 \
		  x/libice \
		  x/libsm \
		  x/libXext \
		  x/libXrender \
		  x/libXaw \
		  common/glib-2.0 \
		  common/ncurses

SRC_TGZ = http://ftp.sunfreeware.com/ftp/pub/freeware/SOURCES/${NAME}-${VERSION}.tar.gz
SRC_TGZ = http://dl.suckless.org/dwm/${NAME}-${VERSION}.tar.gz

# Expect xlibs to be in the xserver package, in the 'xlib' directory.
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xserver
XLIBS = $(APP_DIR)/xlib

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built-${VERSION}

build/%.built-${VERSION}: build/.unpacked-${VERSION}
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	cp config.mk build/$*
	cd build/$* ; ${SB2} $(MAKE) \
		PREFIX=${INSTALL_PREFIX} \
		XLIBS=${XLIBS}
	touch $@

clobber::
	rm -rf build
