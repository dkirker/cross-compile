NAME = xterm
VERSION = 256

DEPENDS = \
		  x/libx11 \
		  x/libice \
		  x/libsm \
		  x/libXext \
		  x/libXrender \
		  x/libXaw \
		  common/glib-2.0 \
		  common/ncurses

SRC_TGZ = ftp://invisible-island.net/${NAME}/${NAME}-${VERSION}.tgz
#We add this to the -rpath so we have the libs X wants
XLIBS = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xlib

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

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	CFLAGS=\"-O2 -I/usr/local/include -I/usr/local/include/ncurses\" \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib\" \
	PATH=\"/usr/local/bin:${PATH}\" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--enable-88-color \
		--enable-256-color \
		--enable-width-chars \
		--enable-luit \
	)	
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install)
	touch $@

clobber::
	rm -rf build
