NAME = glib-2.0
VERSION = 2.16.6
DEPENDS = 

SRC_BZ2 = http://palm.cdnetworks.net/opensource/1.4.1.1/glib-${VERSION}.tar.bz2
PATCH_TGZ = http://palm.cdnetworks.net/opensource/1.4.1.1/glib-${VERSION}-patches.tgz

.PHONY: unpack
unpack: build/.unpacked

DL_DIR = ../../../downloads
SRC_DL = ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
PATCH_DL = ${DL_DIR}/${NAME}-${VERSION}-patches.tgz

.patch.url:
	echo ${PATCH_TGZ} > .patch.url

${PATCH_DL}: .patch.url
	curl ${PATCH_TGZ} > $@.tmp
	mv $@.tmp $@

.src.url:
	echo ${SRC_BZ2} > .src.url

${SRC_DL}: .src.url
	curl ${SRC_BZ2} > $@.tmp
	mv $@.tmp $@

build/.unpacked: ${SRC_DL} ${PATCH_DL}
	rm -rf build
	mkdir -p build
	tar -C build -jxf ${SRC_DL}
	mv build/glib-${VERSION} build/src
	tar xvOzf ${PATCH_DL} | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

GLIB_CFLAGS=""

ifeq ("$(ARCH)","armv7")
GLIB_CFLAGS="-O3 -march=armv7-a -mtune=cortex-a8 -mcpu=cortex-a8 -mfloat-abi=softfp -mfpu=neon -ftree-vectorize -I/usr/local/include"
endif

ifeq ("$(ARCH)","armv6")
GLIB_CFLAGS="-O3 -march=armv6 -mtune=arm1136jf-s -mcpu=arm1136jf-s -mfloat-abi=softfp -mfpu=vfp -ftree-vectorize -I/usr/local/include"
endif

stage-local:: build/${ARCH}.built
	( cd ${STAGING_DIR}/usr/lib ; \
     [ -e libglib-2.0.so ] || ln -s libglib-2.0.so.0.1600.6 libglib-2.0.so; \
     [ -e libgmodule-2.0.so ] || ln -s libgmodule-2.0.so.0.1600.6 libgmodule-2.0.so; \
     [ -e libgobject-2.0.so ] || ln -s libgobject-2.0.so.0.1600.6 libgobject-2.0.so; \
     [ -e libgthread-2.0.so ] || ln -s libgthread-2.0.so.0.1600.6 libgthread-2.0.so )

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*/
	( cd build/$* ; \
	CFLAGS=${GLIB_CFLAGS} \
	LDFLAGS="-Wl,-rpath,/usr/local/lib -L/usr/local/lib" \
		${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/$*; ${SB2} $(MAKE) install-pkgconfigDATA install-configexecincludeDATA ) #this used to be 'install-exec-local', don't see equiv
	( cd build/$*/glib ; ${SB2} $(MAKE) )
	( cd build/$*/glib ; ${SB2} $(MAKE) install-glibincludeHEADERS \
		install-glibsubincludeHEADERS )
	( cd build/$*/gmodule ; ${SB2} $(MAKE) install-glibincludeHEADERS )
	( cd build/$*/gobject ; ${SB2} $(MAKE) install-libgobjectincludeHEADERS )
	( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build

download:: ${SRC_DL} ${PATCH_DL}
