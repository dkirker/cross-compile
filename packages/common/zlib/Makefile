NAME = zlib
VERSION = 1.2.3

SRC_BZ2 = http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}.tar.bz2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}-patches.tgz \
		LOCAL_FILE=${NAME}-${VERSION}-patches.tgz download
	tar -x -f ${DL_DIR}/${NAME}-${VERSION}-patches.tgz -O \
		zlib-patches/visibility.patch zlib-patches/autotools.patch | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

stage-local:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	install -d ${STAGING_DIR}/usr/include
	install -m 644 build/src/zlib.h ${STAGING_DIR}/usr/include/
	install -m 644 build/src/zconf.h ${STAGING_DIR}/usr/include/
	touch $@

clobber::
	rm -f ${STAGING_DIR}/usr/include/zlib.h
	rm -f ${STAGING_DIR}/usr/include/zconf.h
	rm -rf build
