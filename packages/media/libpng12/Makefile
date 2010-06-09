NAME = libpng
VERSION = 1.2.12
DEPENDS = common/zlib


SRC_BZ2 = http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}.tar.bz2

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

include ../../../support/stage-rootfs.mk

stage::
	cp -pP ${ROOTFS_DIR}/usr/lib/libpng12.so* ${STAGING_DIR}/usr/lib
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e ${NAME}.so ] || ln -f -s ${NAME}.so.3.42.0 ${NAME}.so )
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e libpng12.so ] || ln -f -s libpng12.so.0.42.0 libpng12.so )

include ../../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS=-L/usr/local/lib ./configure )
	( cd build/src ; ${SB2} $(MAKE) DESTDIR= install-data )
	touch $@

clobber::
	rm -rf build
	rm -f ${STAGING_DIR}/usr/lib/libpng12.so
	rm -f ${STAGING_DIR}/usr/lib/${NAME}.so
