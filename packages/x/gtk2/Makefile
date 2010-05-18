NAME = gtk+
VERSION = 2.12.11

DEPENDS = x/pango \
		  x/libx11 \
		  x/libice \
		  x/libsm \
		  x/libXext \
		  x/libXrender \
		  common/glib-2.0

SRC_BZ2 = http://ftp.acc.umu.se/pub/gnome/sources/gtk+/2.12/${NAME}-${VERSION}.tar.bz2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage:: build

include ../../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	PKG_CONFIG_LIBDIR=/usr/local/lib/pkgconfig:/usr/local/share/pkgconfig \
	LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--without-libtiff \
		--without-libjpeg \
		--disable-cups \
	)	
	( cd build/src ; ${SB2} make)
	( cd build/src ; ${SB2} make install)
	touch $@

clobber::
	rm -rf build
