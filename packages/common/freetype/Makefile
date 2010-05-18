NAME = freetype
VERSION = 2.3.6

SRC_BZ2 = http://palm.cdnetworks.net/opensource/1.4.1.1/${NAME}-${VERSION}.tar.bz2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	${MAKE} SRC_FILE=http://palm.cdnetworks.net/opensource/1.3.5/${NAME}-${VERSION}-patches.tgz \
		LOCAL_FILE=${NAME}-${VERSION}-patches.tgz download
	tar -x -f ${DL_DIR}/${NAME}-${VERSION}-patches.tgz -O \
		patches/no-hardcode.patch patches/fix-configure.patch patches/libtool-tag.patch | \
		patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure )
	( cd build/src ; ${SB2} make )
	( ${SB2} install -m 777 -d /usr/local/include/freetype )
	( ${SB2} install -m 777 -d /usr/local/include/freetype/config )
	( ${SB2} install -m 777 -d /usr/local/include/freetype/internal )
	( ${SB2} install -m 777 -d /usr/local/include/freetype/internal/services )
	( ${SB2} install -m 644 build/src/include/freetype/*.h \
		/usr/local/include/freetype/ )
	( ${SB2} install -m 644 build/src/include/freetype/config/*.h \
		/usr/local/include/freetype/config/ )
	( ${SB2} install -m 644 build/src/include/freetype/internal/*.h \
		/usr/local/include/freetype/internal/ )
	( ${SB2} install -m 644 build/src/include/freetype/internal/services/*.h \
		/usr/local/include/freetype/internal/services/ )
	( ${SB2} install -m 644 build/src/include/ft2build.h /usr/local/include/ )
	( ${SB2} mkdir -p /usr/local/lib/pkgconfig )
	( ${SB2} install -m 644 build/src/builds/unix/freetype2.pc /usr/local/lib/pkgconfig/ )
	( ${SB2} mkdir -p /usr/local/bin )
	( ${SB2} install -m 755 build/src/builds/unix/freetype-config /usr/local/bin/ )
	touch $@

include ../../../support/stage-rootfs.mk

clobber::
	rm -rf build
