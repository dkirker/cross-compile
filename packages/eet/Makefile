NAME = eet
VERSION = 1.2.3
DEPENDS = zlib jpeg libgcrypt openssl eina

SRC_TGZ = http://xcomputerman.com/files/efl_webos/eet-1.2.3-20100112.tar.gz

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
	export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
	( cd build/src ; ${SB2} ./configure --disable-static --disable-gnutls \
		--prefix=${INSTALL_PREFIX} --disable-doc )
	( cd build/src ; ${SB2} make install )
	touch $@

clobber::
	rm -rf build
