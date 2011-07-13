NAME = xkbset
VERSION = 0.5

DEPENDS = \
	x/libx11 \
	x/libxcb \
	x/xau


SRC_TGZ = http://www.math.missouri.edu/~stephen/software/xkbset/${NAME}-${VERSION}.tar.gz

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
	( cd build/$* ; ${SB2} make \
		CC=gcc \
		LIBS="\"-lX11 -lxcb -lXau -L/usr/local/lib -Wl,-rpath=${XLIBS}\"" \
		X11BASE="/usr/local/lib" \
		X11PREFIX=${XLIBS} \
	)
	( cd build/$* ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
