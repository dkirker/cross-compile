NAME = synergy
VERSION = 1.3.7

DEPENDS = \
		  x/libx11 \
			x/libxtst \
		  common/glib-2.0

SRC_TGZ = http://synergy.googlecode.com/files/$(NAME)-$(VERSION)-Source.tar.gz

# Expect xlibs to be in the xserver package, in the 'xlib' directory.
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xserver
XLIBS = $(APP_DIR)/xlib

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION}-Source build/src
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
	CFLAGS=\"-O2 -I/usr/local/include -I/usr/local/include/ncurses\" \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib\" \
	PATH=\"/usr/local/bin:${PATH}\" \
		cmake . \
			-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
	)
	( cd build/$* ; ${SB2} $(MAKE) )
	touch $@

clobber::
	rm -rf build
