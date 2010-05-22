NAME = pulseaudio
VERSION = 0.9.14
DEPENDS = libsndfile libspeex liboil dbus-lib

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	zcat ${NAME}-${VERSION}-patch.gz | patch -d build/src -p0
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built
	cp -pPr ${ROOTFS_DIR}/usr/lib/libpulse* ${STAGING_DIR}/usr/lib

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} aclocal -I m4 )
	( cd build/src ; ${SB2} autoheader )
	( cd build/src ; ${SB2} autoconf )
	( cd build/src ; ${SB2} libtoolize --automake )
	( cd build/src ; ${SB2} automake --add-missing --copy --gnu )
	( cd build/src ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./configure --disable-static --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} make install-pkgconfigDATA )
	( cd build/src/src ; ${SB2} make install-pulseincludeHEADERS )
	touch $@

clobber::
	rm -rf build
