NAME = scummvm
VERSION = 1.0.0

DEPENDS = dev-sdl/libsdl

SRC_BZ2 = http://sourceforge.net/projects/scummvm/files/scummvm/1.0.0/scummvm-1.0.0.tar.bz2/download

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} ./configure --backend=sdl --with-sdl-prefix=/usr/local/bin \
		--prefix=/media/cryptofs/apps/usr/palm/applications/org.webosinternals.scummvm \
		--enable-plugins --default-dynamic --disable-all-engines \
		--enable-scumm --enable-scumm_7_8 --enable-sky --enable-queen )
	( cd build/$* ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
