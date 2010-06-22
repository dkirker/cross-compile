NAME = libsdl-image
VERSION = 1.2
DEPENDS = dev-sdl/libsdl

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}.tgz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}-patch.gz \
		LOCAL_FILE=${NAME}-${VERSION}-patch.gz download
	zcat ${DL_DIR}/${NAME}-${VERSION}-patch.gz | patch -d build/src -p0
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} PATH=/usr/local/bin:${PATH} ./configure )
	( cd build/src ; ${SB2} $(MAKE) install-data )
	touch $@

clobber::
	rm -rf build
