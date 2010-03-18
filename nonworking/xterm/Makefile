NAME = xterm
VERSION = 256

DEPENDS = \
		  ../nonworking/libx11 \
		  ../nonworking/libice \
		  ../nonworking/libsm \
		  ../nonworking/libXext \
		  ../nonworking/libXrender \
		  ../nonworking/libXaw \
		  glib-2.0 \
		  ncurses

SRC_TGZ = ftp://invisible-island.net/${NAME}/${NAME}-${VERSION}.tgz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

stage:: build

include ../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	PKG_CONFIG_LIBDIR=/usr/local/lib/pkgconfig:/usr/local/share/pkgconfig \
	CFLAGS=\"-O2 -I/usr/local/include -I/usr/local/include/ncurses\" \
	LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--enable-88-color \
		--enable-256-color \
		--enable-width-chars \
		--enable-luit \
	)	
	( cd build/src ; ${SB2} make)
	( cd build/src ; ${SB2} make install)
	touch $@

clobber::
	rm -rf build
