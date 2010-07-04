NAME = libcurl
VERSION = 7.19.4
DEPENDS = common/zlib common/openssl

SRC_TGZ = http://curl.haxx.se/download/curl-7.19.4.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/curl-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} $(MAKE) install-binSCRIPTS install-pkgconfigDATA )
	( cd build/src/include ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
	rm -f ${STAGING_DIR}/usr/lib/libcurl.so
