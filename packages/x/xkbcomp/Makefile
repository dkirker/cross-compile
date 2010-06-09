NAME = xkbcomp
VERSION = 1.1.1

SRC_BZ2 = http://www.x.org/releases/X11R7.5/src/everything/${NAME}-${VERSION}.tar.bz2

#APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xorg-server
#XLIBS = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xlib

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
	( cd build/src ; ${SB2} \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib\" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
	)
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
