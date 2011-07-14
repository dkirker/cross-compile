NAME = oneko
VERSION = 1.2

DEPENDS = x/libXext

SRC_TGZ = http://archive.ubuntu.com/ubuntu/pool/universe/o/${NAME}/${NAME}_${VERSION}.sakura.6.orig.tar.gz

# Expect xlibs to be in the xserver package, in the 'xlib' directory.
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xserver

#We add this to the -rpath so we have the libs X wants
XLIBS = $(APP_DIR)/xlib

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION}.sakura.6.orig build/src
	patch -d build/src -p1 < oneko-${VERSION}.patch
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
	  gcc -o ${NAME} ${NAME}.c \
		-I. -I./cursors -I/usr/local/include/X11 -L/usr/local/lib \
		-Wl,-rpath=${XLIBS}:.:/usr/local/lib -lX11 -lXext -lm -DSHAPE )
	touch $@

clobber::
	rm -rf build
