NAME = liboauth
VERSION = 0.8.8
DEPENDS = common/zlib common/openssl common/curl

SRC_TGZ = http://liboauth.sourceforge.net/pool/liboauth-0.8.8.tar.gz

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

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		./configure --disable-curl --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} $(MAKE) )
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
	rm -f ${STAGING_DIR}/usr/lib/liboauth*
	rm -f ${STAGING_DIR}/usr/lib/pkgconfig/oauth.pc
	rm -f ${STAGING_DIR}/usr/include/oauth.h
	rm -f ${STAGING_DIR}/usr/share/man/man3/oauth.3
