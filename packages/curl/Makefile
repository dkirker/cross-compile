NAME = curl
VERSION = 7.19.4
DEPENDS = zlib openssl

SRC_TGZ = http://curl.haxx.se/download/curl-7.19.4.tar.gz

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
	cp -pP ${ROOTFS_DIR}/usr/lib/libcurl.so* ${STAGING_DIR}/usr/lib
	( cd ${STAGING_DIR}/usr/lib ; \
     [ -e libcurl.so ] || ln -s libcurl.so.4.1.1 libcurl.so )

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} make install-binSCRIPTS install-pkgconfigDATA )
	( cd build/src/include ; ${SB2} make install )
	touch $@

clobber::
	rm -rf build
