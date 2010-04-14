NAME = expat
VERSION = 1.95.8
DEPENDS = 

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.3.5/expat-1.95.8.tar.gz

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

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} /usr/bin/install -c -m 644 lib/expat.h \
		lib/expat_external.h ${INSTALL_PREFIX}/include )
	touch $@

clobber::
	rm -rf build
