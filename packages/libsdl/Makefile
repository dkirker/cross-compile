NAME = libsdl
VERSION = 1.2
DEPENDS = libnapp piranha palmvibelib

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}.tgz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}-patch.gz \
		LOCAL_FILE=${NAME}-${VERSION}-patch.gz download
	zcat ${DL_DIR}/${NAME}-${VERSION}-patch.gz | patch -d build/src -p1
	sed -i.orig -e 's/SDL_GL_SWAP_CONTROL,/SDL_GL_SWAP_CONTROL/' \
		build/src/include/SDL_video.h
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure )
	( cd build/src ; ${SB2} make install-bin install-hdrs install-data )
	touch $@

clobber::
	rm -rf build
