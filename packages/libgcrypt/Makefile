NAME = libgcrypt
VERSION = 1.4.4
DEPENDS = libgpg-error

SRC_TGZ = ftp://ftp.gnupg.org/gcrypt/libgcrypt/libgcrypt-1.4.4.tar.gz

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

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src/src ; ${SB2} make install-includeHEADERS install-binSCRIPTS )
	touch $@

clobber::
	rm -rf build
