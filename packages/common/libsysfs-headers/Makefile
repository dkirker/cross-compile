NAME = sysfsutils
VERSION = 2.0.0
DEPENDS =

SRC_TGZ = http://palm.cdnetworks.net/opensource/3.0.2/${NAME}-${VERSION}.tar.gz

HEADER_FILES = dlist.h libsysfs.h

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage-local:: build/.unpacked
	mkdir -p ${STAGING_DIR}/usr/include
	$(foreach header_file,${HEADER_FILES}, \
		install -m 644 build/src/include/${header_file} ${STAGING_DIR}/usr/include/${header_file};\
	)

clobber::
	rm -rf build
	$(foreach header_file,${HEADER_FILES}, \
		rm -f ${STAGING_DIR}/usr/include/${header_file};\
	)
