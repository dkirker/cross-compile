NAME = lua
VERSION = 5.1.4
DEPENDS =  readline ncurses

SRC_TGZ = http://www.lua.org/ftp/lua-5.1.4.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	zcat ${NAME}-${VERSION}-patches.gz | patch -d build/src -p1
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} make linux )
	( cd build/src ; ${SB2} make install )
	( cd build/src/etc ; ${SB2} install -m 644 lua.pc /usr/local/lib/pkgconfig )
	touch $@

clobber::
	rm -rf build
