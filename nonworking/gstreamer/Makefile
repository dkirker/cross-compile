NAME = gstreamer
VERSION = 0.10.21
DEPENDS = glib-2.0 ../nonworking/bison ../nonworking/flex


SRC_TGZ = http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}.tgz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}-patch.gz \
		LOCAL_FILE=${NAME}-${VERSION}-patch.gz download
	zcat ${DL_DIR}/${NAME}-${VERSION}-patch.gz | patch -d build/src -p0
	#cp build/src/configure build/src/configure.orig && sed -e 's/2.12/2.0/g' build/src/configure.orig > build/src/configure
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; LDFLAGS="-Wl,-rpath,/usr/local/lib -L/usr/local/lib" ${SB2} ./configure --prefix=/usr/local --disable-static --disable-dependency-tracking --disable-valgrind --disable-examples )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	touch $@

clobber::
	rm -rf build
