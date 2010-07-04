NAME = libgcrypt
VERSION = 1.4.4
DEPENDS = common/libgpg-error

SRC_TGZ = ftp://ftp.gnupg.org/gcrypt/libgcrypt/libgcrypt-1.4.4.tar.gz

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
	( cd build/src ; ${SB2} ./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src/src ; ${SB2} $(MAKE) install-includeHEADERS install-binSCRIPTS )
	( ${SB2} ln -f -s libgcrypt.so.11.5.2 ${INSTALL_PREFIX}/lib/libgcrypt.so )
	touch $@

clobber::
	rm -rf build
