NAME = pulseaudio
VERSION = 0.9.14
DEPENDS = media/libsndfile media/libspeex common/liboil common/dbus-lib common/gdbm

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}.tgz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	zcat ${NAME}-${VERSION}-patch.gz | patch -d build/src -p0
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built
	cp -pPr ${ROOTFS_DIR}/usr/lib/libpulse* ${STAGING_DIR}/usr/lib

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	# without that aclocal call some of the macros are not found -- Malte Schröder
	( cd build/$* ; ${SB2} aclocal -I m4 -I ${STAGING_DIR}/usr/share/aclocal )
	# don't know if the following 4 lines are necessary, taken from bootstrap.sh. Works for me -- Malte Schröder
	( cd build/$* ; ${SB2} autoconf -Wall )
	( cd build/$* ; ${SB2} autoheader )
	( cd build/$* ; ${SB2} libtoolize --auto$(MAKE) )
	( cd build/$* ; ${SB2} automake --copy --foreign --add-missing )
	( cd build/$* ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		./configure --prefix=${INSTALL_PREFIX} )
	( cd build/$* ; ${SB2} $(MAKE) install-pkgconfigDATA )
	( cd build/$*/src ; ${SB2} $(MAKE) install-pulseincludeHEADERS )
	touch $@

clobber::
	rm -rf build
