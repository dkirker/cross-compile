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

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} make install-binSCRIPTS install-pkgconfigDATA )
	( cd build/src/include ; ${SB2} make install )
	touch $@

clobber::
	rm -rf build
