NAME = libxml2
VERSION = 2.6.26
DEPENDS = 

SRC_TGZ = ftp://xmlsoft.org/libxml2/old/libxml2-2.6.26.tar.gz

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	rm -rf build
	mkdir -p build
	curl ${SRC_TGZ} | tar -C build -zxf -
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} make install-pkgconfigDATA )
	( cd build/src/include ; ${SB2} make install )
	touch $@

clobber::
	rm -rf build
