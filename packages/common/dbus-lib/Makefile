NAME = dbus-lib
VERSION = 1.1.20
DEPENDS = common/expat

SRC_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/dbus-1.1.20.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/dbus-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.4.1.1/dbus-${VERSION}-patches.tgz \
		LOCAL_FILE=${NAME}-${VERSION}-patches.tgz download
	tar -zxOf ${DL_DIR}/${NAME}-${VERSION}-patches.tgz | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e libdbus-1.so ] || ln -f -s libdbus-1.so.3.4.0 libdbus-1.so )

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} export CFLAGS=-I${INSTALL_PREFIX}/include && \
		export LDFLAGS="-L${INSTALL_PREFIX}/lib -Wl,-rpath,${INSTALL_PREFIX}/lib" && \
		./configure --disable-static --prefix=${INSTALL_PREFIX} \
		--with-xml-expat )
	( cd build/$* ; ${SB2} $(MAKE) install-pkgconfigDATA )
	( cd build/$*/dbus ; ${SB2} $(MAKE) install-dbusincludeHEADERS \
		install-dbusarchincludeHEADERS )
	touch $@

clobber::
	rm -rf build
	rm -f ${STAGING_DIR}/usr/lib/libdbus-1.so
