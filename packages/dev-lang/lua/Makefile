NAME = lua
VERSION = 5.1.4
DEPENDS = common/readline common/ncurses

SRC_TGZ = http://www.lua.org/ftp/lua-5.1.4.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	zcat ${NAME}-${VERSION}-patches.gz | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} $(MAKE) linux )
	( cd build/$* ; ${SB2} $(MAKE) install )
	( cd build/$*/etc ; ${SB2} install -d ${INSTALL_PREFIX}/lib/pkgconfig )
	( cd build/$*/etc ; ${SB2} install -m 644 lua.pc ${INSTALL_PREFIX}/lib/pkgconfig )
	touch $@

clobber::
	rm -rf build
