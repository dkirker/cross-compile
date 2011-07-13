NAME = openssl
VERSION = 0.9.8k
DEPENDS = common/glib-2.0

SRC_TGZ = http://www.openssl.org/source/openssl-0.9.8k.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built
	(cd ${STAGING_DIR}/usr/lib ; \
		[ -e libcrypto.so ] || ln -sf libcrypto.so.0.9.8 libcrypto.so ; \
		[ -e libssl.so ] || ln -sf libssl.so.0.9.8 libssl.so ; \
	)

OPENSSL_LIBS = libcrypto libssl

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} ./Configure --prefix=${INSTALL_PREFIX} linux-elf )
	( cd build/$* ; ${SB2} $(MAKE) libcrypto.pc libssl.pc openssl.pc )
	( cd build/$* ; ${SB2} /usr/bin/install -d ${INSTALL_PREFIX}/lib/pkgconfig )
	( cd build/$* ; ${SB2} /usr/bin/install -m 644 libcrypto.pc libssl.pc \
		openssl.pc ${INSTALL_PREFIX}/lib/pkgconfig )
	( cd build/$*/include ; ${SB2} cp -Lr openssl ${INSTALL_PREFIX}/include )
	# Automatic staging doesn't work for OpenSSL libs
	cp -pP $(foreach LIB,${OPENSSL_LIBS},${ROOTFS_DIR}/usr/lib/${LIB}.so.*) ${STAGING_DIR}/usr/lib/
	touch $@

clobber::
	-rm -rf build
	-rm ${STAGING_DIR}/usr/lib/libcrypt.so
	-rm ${STAGING_DIR}/usr/lib/libssl.so
