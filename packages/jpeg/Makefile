NAME = jpeg
VERSION = 6b
DEPENDS = 


SRC_TGZ = http://palm.cdnetworks.net/opensource/1.3.5/${NAME}src.v${VERSION}.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.3.5/${NAME}src.v${VERSION}-patches.tgz \
		LOCAL_FILE=${NAME}-${VERSION}-patches.tgz download
	tar -x -f ${DL_DIR}/${NAME}-${VERSION}-patches.tgz -O \
		patches/debian.patch patches/ldflags.patch patches/paths.patch | patch -d build/src -p1
	zcat ${DL_DIR}/${NAME}-${VERSION}-patch.gz | patch -d build/src -p1
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure )
	( cd build/src ; ${SB2} make DESTDIR= install-headers )
	touch $@

clobber::
	rm -rf build
