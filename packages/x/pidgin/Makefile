NAME = pidgin
VERSION = 2.6.6

DEPENDS = common/libgcrypt \
		  common/gnutls    \
		  x/gtk2

SRC_BZ2 = http://sourceforge.net/projects/pidgin/files/Pidgin/${NAME}-${VERSION}.tar.bz2

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
	PYTHON=/usr/bin/python \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--disable-screensaver \
		--disable-dbus \
		--disable-sm \
		--disable-startup-notification \
		--disable-gtkspell \
		--disable-gstreamer \
		--disable-vv \
		--disable-idn \
		--disable-meanwhile \
		--disable-avahi \
		--disable-nm \
		--disable-perl \
		--disable-tcl \
	)	
	( cd build/src ; ${SB2} make)
	( cd build/src ; ${SB2} make install)
	touch $@

clobber::
	rm -rf build
