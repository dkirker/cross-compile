NAME = libgpg-error
VERSION = 1.4
DEPENDS = 

SRC_TGZ = ftp://ftp.gnupg.org/gcrypt/libgpg-error/libgpg-error-1.4.tar.gz

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

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} ./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/$*/src ; ${SB2} $(MAKE) install-includeHEADERS install-binSCRIPTS )
	touch $@

clobber::
	rm -rf build
