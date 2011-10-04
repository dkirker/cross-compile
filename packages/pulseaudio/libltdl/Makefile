NAME = libltdl
VERSION = 1.5.24
DEPENDS = 

SRC_TGZ = http://ftp.gnu.org/gnu/libtool/libtool-${VERSION}.tar.gz

.PHONY: unpack
unpack: build/.unpacked

include ../../../support/download.mk

include ../../../support/staging.mk

#include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

build/.unpacked: ${DL_DIR}/libltdl-${VERSION}.tar.gz
	rm -f $@
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/libtool-${VERSION} build/src
	touch $@

stage-local:: build/${ARCH}.built
	( cp -f ../../common/libtool/build/src/libltdl/ltdl.h ${STAGING_DIR}/usr/include/ )

build/%.built: build/.unpacked
	rm -f $@
	rm -rf build/$*
	mkdir -p build/$*
	cp -R build/src/* build/$*/
	( cd build/$* ; cd libltdl ; \
	LDFLAGS="-Wl,-rpath,/usr/local/lib -L/usr/local/lib" \
		${SB2} ./configure --prefix=${INSTALL_PREFIX} --enable-ltdl-install; \
		${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
