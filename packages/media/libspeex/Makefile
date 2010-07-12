NAME = libspeex
VERSION = 1.2rc1
DEPENDS = 

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/speex-1.2rc1.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/speex-1.2rc1 build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} export CFLAGS=-I${INSTALL_PREFIX}/include && \
		export LDFLAGS="-L${INSTALL_PREFIX}/lib -Wl,-rpath,${INSTALL_PREFIX}/lib" && \
		./configure --prefix=${INSTALL_PREFIX} --disable-static )
	( cd build/$* ; ${SB2} $(MAKE) install-pkgconfigDATA )
	( cd build/$*/include ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
