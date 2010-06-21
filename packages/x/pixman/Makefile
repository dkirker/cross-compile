NAME = pixman
VERSION = 0.17.8

DEPENDS = x/fontconfig

SRC_BZ2 = http://cgit.freedesktop.org/pixman/snapshot/${NAME}-${VERSION}.tar.bz2

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

#The sed line might be "needed" due to incompatible libtool on my machine...
build/%.built: build/.unpacked
	( cd build/src ; ${SB2} \
	./autogen.sh \
		--prefix=${INSTALL_PREFIX} \
		--disable-arm-neon \
		--disable-gtk \
	)
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
