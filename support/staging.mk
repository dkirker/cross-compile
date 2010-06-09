ifeq ("${ARCH}", "")
ARCH = armv7
endif

STAGING_DIR := $(shell cd ../../../staging/${ARCH}; pwd)

MAPPING_FILE := $(shell cd ../../../staging; pwd)/mapping-${ARCH}

ROOTFS_DIR := $(shell cd ../../../rootfs/${ARCH}; pwd)

IPKG_FILES_LIST = ${ROOTFS_DIR}/usr/lib/ipkg/info/${NAME}.list

INSTALL_PREFIX = /usr/local

ifneq ("${DEPENDS}","")
stage:: stage-deps

stage-deps: build/${ARCH}.staged-deps

build/${ARCH}.staged-deps: Makefile
	rm -f $@
	mkdir -p build
	for dep in ${DEPENDS} ; do \
	  ${MAKE} -C ../../$$dep stage || exit 1 ; \
	done
	touch $@
endif
