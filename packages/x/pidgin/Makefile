NAME = pidgin
VERSION = 2.6.6

DEPENDS = common/libgcrypt \
		  common/gnutls    \
		  x/gtk2
SRC_BZ2 = http://sourceforge.net/projects/pidgin/files/Pidgin/${VERSION}/${NAME}-${VERSION}.tar.bz2/download

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

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} \
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
	( cd build/$* ; ${SB2} $(MAKE))
	( cd build/$* ; ${SB2} $(MAKE) install)
	touch $@

clobber::
	rm -rf build
