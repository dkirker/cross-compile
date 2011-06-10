NAME = fuse
VERSION = 2.7.2
DEPENDS = 

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/fuse-2.7.2.tar.gz
PATCH_TGZ =  http://palm.cdnetworks.net/opensource/1.4.1.1/fuse-2.7.2-patches.tgz

include ../../../support/download.mk

.PHONY: unpack patch
unpack: build/.unpacked
patch: build/.patched

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=${PATCH_TGZ} \
		LOCAL_FILE=${NAME}-${VERSION}-patches.tgz download
	tar -zxOf ${DL_DIR}/${NAME}-${VERSION}-patches.tgz | patch -d build/src -p1
	patch -d build/src -p1 < ltmain.sh.patch
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2}	./configure --prefix=${INSTALL_PREFIX} --disable-kernel-module --disable-util )
	( cd build/$* ; ${SB2} $(MAKE) )
	( cd build/$* ; ${SB2} $(MAKE) install-data )
	touch $@

clobber::
	rm -rf build
