NAME = glib-2.0
VERSION = 2.14.4
DEPENDS = 

SRC_BZ2 = http://palm.cdnetworks.net/opensource/1.3.5/glib-${VERSION}.tar.bz2
PATCH = http://palm.cdnetworks.net/opensource/1.3.5/glib-${VERSION}-patches.tgz

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: 
	rm -rf build
	mkdir -p build
	curl ${SRC_BZ2} | tar -C build -jxf -
	mv build/glib-${VERSION} build/src
	curl ${PATCH} | tar xvOf - | patch -d build/src -p1
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built
	( cd ${STAGING_DIR}/usr/lib ; \
     [ -e libglib-2.0.so ] || ln -s libglib-2.0.so.0.1400.4 libglib-2.0.so )

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2}	./configure --prefix=${INSTALL_PREFIX} )
	( cd build/src; ${SB2} make install-pkgconfigDATA install-exec-local )
	( cd build/src/glib ; ${SB2} make install-glibincludeHEADERS \
		install-glibsubincludeHEADERS )
	( cd build/src/gmodule ; ${SB2} make install-glibincludeHEADERS )
	touch $@

clobber::
	rm -rf build
