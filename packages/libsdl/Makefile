NAME = libsdl
VERSION = 1.2.13

SRC_TGZ = http://www.libsdl.org/release/SDL-${VERSION}.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/SDL-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.3.5/libsdl-1.2-patch.gz \
		LOCAL_FILE=libsdl-1.2-patch.gz download
	zcat ${DL_DIR}/libsdl-1.2-patch.gz | patch -d build/src -p1
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	( cd build/src ; sb2 -t $* ./configure )
	( cd build/src ; \
	  sb2 -t $* make prefix=/usr DESTDIR=/srv/preware/cross-compile/staging/$* install-hdrs )

clobber::
	rm -rf build
