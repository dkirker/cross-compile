NAME = libpng
VERSION = 1.2.12
DEPENDS = zlib


SRC_BZ2 = http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}.tar.bz2

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

stage::
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e ${NAME}.so ] || ln -s ${NAME}.so.3.12.0 ${NAME}.so )

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS=-L/usr/local/lib ./configure )
	( cd build/src ; ${SB2} make DESTDIR= install-data )
	touch $@

clobber::
	rm -rf build
