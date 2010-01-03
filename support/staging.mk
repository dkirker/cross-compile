ifeq ("${ARCH}", "")
ARCH = armv7
endif

STAGING_DIR = ../../staging/${ARCH}

ROOTFS_DIR = ../../rootfs/${ARCH}

IPKG_FILES_LIST = ${ROOTFS_DIR}/usr/lib/ipkg/info/${NAME}.list

