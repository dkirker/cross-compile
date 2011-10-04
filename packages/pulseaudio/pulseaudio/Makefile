NAME = pulseaudio
VERSION = 0.9.22
DEPENDS = common/libtool media/libspeex pulseaudio/libsndfile1 pulseaudio/libltdl 

INSTALL_PREFIX = /usr

SRC_TGZ = http://palm.cdnetworks.net/opensource/3.0.2/${NAME}-${VERSION}.tgz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/3.0.2/${NAME}-${VERSION}-patch.gz \
		LOCAL_FILE=${NAME}-${VERSION}-patch.gz download
	zcat ${DL_DIR}/${NAME}-${VERSION}-patch.gz | patch -d build/src -p0
	cat ./patches/pulseaudio-autofoofix.patch | patch -d build/src -p1
	cat ./patches/pulseaudio-modifications.patch | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built
#	( cd ${STAGING_DIR}/usr/lib ; \
#	  [ -e libgdbm.so ] || ln -f -s libgdbm.so.3.0.0 libgdbm.so )

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} ./configure --prefix=${INSTALL_PREFIX} --disable-alsa --disable-dbus )
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
#	rm -f ${STAGING_DIR}/usr/lib/libgdbm.so
