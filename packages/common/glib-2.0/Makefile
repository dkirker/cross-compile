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
	curl ${PATCH} | tar xvOzf - | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built
	( cd ${STAGING_DIR}/usr/lib ; \
     [ -e libglib-2.0.so ] || ln -s libglib-2.0.so.0.1400.4 libglib-2.0.so; \
     [ -e libgmodule-2.0.so ] || ln -s libgmodule-2.0.so.0.1400.4 libgmodule-2.0.so; \
     [ -e libgobject-2.0.so ] || ln -s libgobject-2.0.so.0.1400.4 libgobject-2.0.so; \
     [ -e libgthread-2.0.so ] || ln -s libgthread-2.0.so.0.1400.4 libgthread-2.0.so )

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ;  LDFLAGS="-Wl,-rpath,/usr/local/lib -L/usr/local/lib" ${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/src; ${SB2} make install-pkgconfigDATA install-exec-local )
	( cd build/src/glib ; ${SB2} make )
	( cd build/src/glib ; ${SB2} make install-glibincludeHEADERS \
		install-glibsubincludeHEADERS )
	( cd build/src/gmodule ; ${SB2} make install-glibincludeHEADERS )
	( cd build/src/gobject ; ${SB2} make install-libgobjectincludeHEADERS )
	( cd build/src ; ${SB2} make install )
	touch $@

clobber::
	rm -rf build
