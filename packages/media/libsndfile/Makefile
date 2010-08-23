NAME = libsndfile1
VERSION = 1.0.17
DEPENDS = media/alsa-lib

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/libsndfile-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack patch
unpack: build/.unpacked
patch: build/.patched

build/.unpacked: ${DL_DIR}/libsndfile1-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/libsndfile-${VERSION} build/src
	# Patch throws m4 into an infinite loop, don't need it anyway
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.4.1.1/libsndfile-${VERSION}-patches.tgz \
		LOCAL_FILE=libsndfile-${VERSION}-patches.tgz download
	tar -zxOf ${DL_DIR}/libsndfile-${VERSION}-patches.tgz | patch -d build/src -p1
	touch $@

build/.patched: build/.unpacked
      	rm -f $@
       	( cd build/src && patch -p0 -i../../acinclude.patch )
       	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built
	( cd ${STAGING_DIR}/usr/lib ; \
     [ -e libsndfile.so ] || ln -s libsndfile.so.1.0.17 libsndfile.so )

build/%.built: build/.unpacked build/.patched
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} export CFLAGS=-I${INSTALL_PREFIX}/include && \
		export LDFLAGS="-L${INSTALL_PREFIX}/lib -Wl,-rpath,${INSTALL_PREFIX}/lib" && \
		./configure --disable-static --prefix=${INSTALL_PREFIX} \
		--with-xml-expat )
	( cd build/$* ; ${SB2} $(MAKE) install-pkgconfigDATA )
	( cd build/$*/src ; ${SB2} $(MAKE) install-includeHEADERS )
	${SB2} cp build/$*/src/sndfile.h ${INSTALL_PREFIX}/include
	touch $@

clobber::
	rm -rf build
