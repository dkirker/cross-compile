NAME = ncurses
VERSION = 5.4
DEPENDS = 

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}-patches.tgz \
		LOCAL_FILE=${NAME}-${VERSION}-patches.tgz download
	zcat ${DL_DIR}/${NAME}-${VERSION}-patches.tgz | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e libncurses.so ] || ln -f -s libncurses.so.5.4 libncurses.so )

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/src/include ; ${SB2} make install )
	touch $@

clobber::
	rm -rf build
	rm -f ${STAGING_DIR}/usr/lib/libncurses.so
