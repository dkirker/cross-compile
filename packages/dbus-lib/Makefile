NAME = dbus
VERSION = 1.1.20
DEPENDS = expat

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.3.5/dbus-1.1.20.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}-patches.tgz \
		LOCAL_FILE=${NAME}-${VERSION}-patches.tgz download
	tar -zxOf ${DL_DIR}/${NAME}-${VERSION}-patches.tgz | patch -d build/src -p1
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e libdbus-1.so ] || ln -f -s libdbus-1.so.3.4.0 libdbus-1.so )

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} export CFLAGS=-I${INSTALL_PREFIX}/include && \
		export LDFLAGS="-L${INSTALL_PREFIX}/lib -Wl,-rpath,${INSTALL_PREFIX}/lib" && \
		./configure --disable-static --prefix=${INSTALL_PREFIX} \
		--with-xml-expat )
	( cd build/src ; ${SB2} make install-pkgconfigDATA )
	( cd build/src/dbus ; ${SB2} make install-dbusincludeHEADERS \
		install-dbusarchincludeHEADERS )
	touch $@

clobber::
	rm -rf build
	rm -f ${STAGING_DIR}/usr/lib/libdbus-1.so
