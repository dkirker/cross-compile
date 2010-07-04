NAME = libxml2
VERSION = 2.6.26
DEPENDS = 

SRC_TGZ = ftp://xmlsoft.org/libxml2/old/libxml2-2.6.26.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -zxf $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; LDFLAGS="-Wl,-rpath,/usr/local/lib -L/usr/local/lib" ${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} $(MAKE) install-pkgconfigDATA )
	( cd build/src/include ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
