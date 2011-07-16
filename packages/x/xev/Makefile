NAME = xev
VERSION = 1.1.0

DEPENDS = x/libxcb x/xau

SRC_BZ2 = http://www.x.org/releases/X11R7.6/src/everything/${NAME}-${VERSION}.tar.bz2

# Expect xlibs to be in the xserver package, in the 'xlib' directory.
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xserver

#We add this to the -rpath so we have the libs X wants
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
	CFLAGS=\"-I/usr/local/include\" \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib\" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
	)
	( cd build/$* ; ${SB2} $(MAKE))
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
