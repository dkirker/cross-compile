NAME = eina
VERSION = 0.9.9.063
DEPENDS = 

SRC_TGZ = http://xcomputerman.com/files/efl_webos/eina-0.9.9.063-20100112.tar.gz

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
	( cd build/src ; ${SB2} ./configure --disable-static --prefix=${INSTALL_PREFIX} \
		--disable-doc )
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src ; ${SB2} make install )
	touch $@

clobber::
	rm -rf build
