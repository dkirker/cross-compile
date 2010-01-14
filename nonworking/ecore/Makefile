NAME = ecore
VERSION = 0.9.9.063
DEPENDS = libsdl openssl gnutls fontconfig curl eina eet evas

SRC_TGZ = http://xcomputerman.com/files/efl_webos/ecore-0.9.9.063-20100112.tar.gz

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
	( cd build/src ; PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		${SB2} ./configure --prefix=${INSTALL_PREFIX} \
		--disable-static --enable-ecore-sdl \
		--enable-ecore-evas-software-sdl \
		--enable-ecore-fb --enable-ecore-evas-fb \
		--disable-doc )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	touch $@

clobber::
	rm -rf build
