NAME = xkbcomp
VERSION = 1.2.0

DEPENDS = x/libx11 x/xkbfile

SRC_BZ2 = http://www.x.org/releases/X11R7.6/src/everything/${NAME}-${VERSION}.tar.bz2

# Expect xlibs to be in the xserver package, in the 'xlib' directory.
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xserver
XLIBS = $(APP_DIR)/xlib

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
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
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib\" \
	./configure \
		--prefix=${APP_DIR} \
	)
	( cd build/$* ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
