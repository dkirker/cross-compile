NAME = libsndfile1
VERSION = 1.0.21
DEPENDS = 

SRC_TGZ = http://palm.cdnetworks.net/opensource/3.0.2/libsndfile-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/libsndfile-${VERSION} build/src
#	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/3.0.2/libsndfile-${VERSION}-patches.tgz \
#		LOCAL_FILE=libsndfile-${VERSION}-patches.tgz download
#	zcat ${DL_DIR}/libsndfile-${VERSION}-patches.tgz | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e libsndfile.so ] || ln -f -s libsndfile.so.1.0.17 libsndfile.so )

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
	rm -f ${STAGING_DIR}/usr/lib/libsndfile.so
