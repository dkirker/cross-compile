NAME = libGLUES_CM
VERSION = 51
DEPENDS = dev-sdl/libsdl

SRC_SVN_REV = http://glues.googlecode.com/svn/trunk/glues

include ../../../support/download.mk

CC = gcc

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -xf ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz
	cat ${NAME}-svn-r${VERSION}-patch | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

stage-local:: build/${ARCH}.built
	install -d ${STAGING_DIR}/usr/include
	install -d ${STAGING_DIR}/usr/lib
	rm -f ${STAGING_DIR}/usr/include/glues.h
	install -m 644 build/src/source/glues.h ${STAGING_DIR}/usr/include/
	rm -f ${STAGING_DIR}/usr/include/glu.h
	ln -f -s glues.h ${STAGING_DIR}/usr/include/glu.h
	rm -f ${STAGING_DIR}/usr/lib/libGLUES_CM.so
	install -m 755 build/src/source/libGLUES_CM.so ${STAGING_DIR}/usr/lib/

include ../../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src/source ; ${SB2} $(MAKE) )
	touch $@

clobber::
	rm -f ${STAGING_DIR}/usr/lib/libGLUES_CM.so
	rm -f ${STAGING_DIR}/usr/include/glu.h
	rm -f ${STAGING_DIR}/usr/include/glues.h
	rm -rf build

