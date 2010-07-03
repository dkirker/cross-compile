NAME = libGLUES_CM
VERSION = 51
DEPENDS = dev-gl/libGLUES_CM common/tisgxgfx common/adrenogfx

SRC_SVN_REV = http://glues.googlecode.com/svn/trunk/glues

include ../../../support/download.mk

CC = gcc

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -xf ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz
	cat ${NAME}-svn-r${VERSION}-demos-patch | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

stage-local:: build/${ARCH}.built

include ../../../support/cross-compile.mk

build: stage

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	[ "${ARCH}" != "armv6" ] || ( cd build/$*/sdltests ; sed -i "s/-lGLES_CM/-lGLES_CM -legl13/" Makefile )
	( cd build/$*/sdltests ; ${SB2} $(MAKE) )
	touch $@

clobber::
	rm -rf build

