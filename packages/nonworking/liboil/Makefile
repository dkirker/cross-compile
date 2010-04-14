NAME = liboil
VERSION = 0.3.14
DEPENDS = 

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	touch $@

clobber::
	rm -rf build
