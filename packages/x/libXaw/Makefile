NAME = libXaw
VERSION = 1.0.7

DEPENDS = \
		x/xorg-server \
		x/libxt \
		x/libXmu \
		x/libXpm

SRC_BZ2 = http://www.x.org/releases/X11R7.5/src/everything/${NAME}-${VERSION}.tar.bz2

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
	PATH=/usr/local/bin/:${PATH} \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--disable-docs \
	)
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
