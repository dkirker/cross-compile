NAME = expat
VERSION = 1.95.8
DEPENDS = 

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/expat-1.95.8.tar.gz

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
	${SB2} install -d ${INSTALL_PREFIX}/include
	( cd build/$* ; ${SB2} /usr/bin/install -c -m 644 lib/expat.h \
		lib/expat_external.h ${INSTALL_PREFIX}/include )
	touch $@

stage-local::
	cp -pP ${ROOTFS_DIR}/usr/lib/libexpat.so* ${STAGING_DIR}/usr/lib
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e lib${NAME}.so ] || ln -f -s lib${NAME}.so.0 lib${NAME}.so )

clobber::
	rm -rf build ${STAGING_DIR}/usr/lib/lib${NAME}.so*
