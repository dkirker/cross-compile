NAME = libgpg-error
VERSION = 1.4
DEPENDS = 

SRC_TGZ = ftp://ftp.gnupg.org/gcrypt/libgpg-error/libgpg-error-1.4.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src/src ; ${SB2} make install-includeHEADERS install-binSCRIPTS )
	( ${SB2} cp -pP ${ROOTFS_DIR}/usr/lib/libgpg-error.so* ${INSTALL_PREFIX}/lib )
	touch $@

clobber::
	rm -rf build
