NAME = openssl
VERSION = 0.9.8k
DEPENDS = 

SRC_TGZ = http://www.openssl.org/source/openssl-0.9.8k.tar.gz

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

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

OPENSSL_LIBS = libcrypto libssl

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./Configure --prefix=${INSTALL_PREFIX} linux-elf )
	( cd build/src ; ${SB2} make libcrypto.pc libssl.pc openssl.pc )
	( cd build/src ; /usr/bin/install -m 644 libcrypto.pc libssl.pc \
		openssl.pc ${INSTALL_PREFIX}/lib/pkgconfig )
	( cd build/src/include ; cp -Lr openssl ${INSTALL_PREFIX}/include )
	# Automatic staging doesn't work for OpenSSL libs
	cp -pP $(foreach LIB,${OPENSSL_LIBS},${ROOTFS_DIR}/usr/lib/${LIB}.so.*) ${STAGING_DIR}/usr/lib/
	touch $@

clobber::
	rm -rf build
