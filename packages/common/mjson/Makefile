NAME = mjson
VERSION = 1.0

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/mjson-1.0.tgz
PATCH = http://palm.cdnetworks.net/opensource/1.4.1.1/mjson-1.0-patch.gz

include ../../../support/download.mk

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	curl ${PATCH} | zcat | patch -d build/src -p0
	touch $@

stage-local:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	${SB2} install -d ${INSTALL_PREFIX}/include
	( ${SB2} install -m 644 build/src/json.h ${INSTALL_PREFIX}/include/json.h )
	touch $@

clobber::
	rm -rf build
