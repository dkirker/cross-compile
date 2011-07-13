ifeq ("${ARCH}", "")
ARCH = armv7
endif

STAGING_DIR := $(shell cd ../../..; pwd)/staging/${ARCH}

MAPPING_FILE := $(shell cd ../../..; pwd)/staging/mapping-${ARCH}

ROOTFS_DIR := $(shell cd ../../..; pwd)/rootfs/${ARCH}

IPKG_FILES_LIST = ${ROOTFS_DIR}/usr/lib/ipkg/info/${NAME}.list

INSTALL_PREFIX = /usr/local

include ../../../support/depends.mk
