NAME = mjson
VERSION = 1.0

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/mjson-1.0.tgz
PATCH = http://palm.cdnetworks.net/opensource/1.4.1.1/mjson-1.0-patch.gz

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

.PHONY: unpack
unpack: build/.unpacked

DL_DIR = ../../../downloads
SRC_DL = ${DL_DIR}/${NAME}-${VERSION}.tar.gz
PATCH_DL = ${DL_DIR}/${NAME}-${VERSION}-patches.tgz

${PATCH_DL}:
	curl ${PATCH} > $@.tmp
	mv $@.tmp $@

${SRC_DL}:
	curl ${SRC_TGZ} > $@.tmp
	mv $@.tmp $@

build/.unpacked: ${SRC_DL} ${PATCH_DL}
	rm -rf build
	mkdir -p build
	tar -C build -x -f ${SRC_DL}
	mv build/${NAME}-${VERSION} build/src
	zcat ${PATCH_DL} | patch -d build/src -p0
	touch $@

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	${SB2} install -d ${INSTALL_PREFIX}/include
	( ${SB2} install -m 644 build/src/json.h ${INSTALL_PREFIX}/include/json.h )
	touch $@

clobber::
	rm -rf build

download:: ${PATCH_DL} ${SRC_DL}
