NAME = libtool
VERSION = 2.4
DEPENDS = 

SRC_TGZ = http://ftp.gnu.org/gnu/libtool/${NAME}-${VERSION}.tar.gz

.PHONY: unpack
unpack: build/.unpacked

include ../../../support/download.mk

include ../../../support/staging.mk

#include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -f $@
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

stage-local:: build/${ARCH}.built
#	( cd ${STAGING_DIR}/usr/lib ; \
#     [ -e libglib-2.0.so ] || ln -s libglib-2.0.so.0.1600.6 libglib-2.0.so; \
#     [ -e libgmodule-2.0.so ] || ln -s libgmodule-2.0.so.0.1600.6 libgmodule-2.0.so; \
#     [ -e libgobject-2.0.so ] || ln -s libgobject-2.0.so.0.1600.6 libgobject-2.0.so; \
#    [ -e libgthread-2.0.so ] || ln -s libgthread-2.0.so.0.1600.6 libgthread-2.0.so )

build/%.built: build/.unpacked
	rm -f $@
	rm -rf build/$*
	mkdir -p build/$*
	cp -R build/src/* build/$*/
	( cd build/$* ; \
	LDFLAGS="-Wl,-rpath,/usr/local/lib -L/usr/local/lib" \
		${SB2} ./configure --prefix=${INSTALL_PREFIX}; \
		${SB2} $(MAKE) install-libLTLIBRARIES install-ltdlincludeHEADERS )
	touch $@

clobber::
	rm -rf build
