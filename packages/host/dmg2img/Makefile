NAME = dmg2img
VERSION = 1.6.2
DEPENDS = 

SRC_TGZ = http://vu1tur.eu.org/tools/download.pl?dmg2img-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	patch -d build/${NAME}-${VERSION} -p0 < ${NAME}.patch
	mv build/${NAME}-${VERSION} build/src
	touch $@

build:: build/host.built

build/host.built: build/.unpacked
	rm -f $@
	( cd build/src ; $(MAKE) )
	touch $@

clobber::
	rm -rf build
