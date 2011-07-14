NAME = xterm
VERSION = 270

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
	( cd build/$* ; ${SB2} \
	CFLAGS=\"-O2 -I/usr/local/include -I/usr/local/include/ncurses\" \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib\" \
	PATH=\"/usr/local/bin:${PATH}\" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--enable-88-color \
		--enable-256-color \
		--disable-luit \
		--disable-wide-chars \
	)	
	( cd build/$* ; ${SB2} $(MAKE))
	( cd build/$* ; ${SB2} $(MAKE) install)
	touch $@

clobber::
	rm -rf build
