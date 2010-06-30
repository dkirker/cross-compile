NAME = dosbox
VERSION = 0.73

DEPENDS = dev-sdl/libsdl

SRC_BZ2 = http://prdownloads.sourceforge.net/dosbox/dosbox-0.73.tar.gz?download

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	( cd build/src ; cat ../../*patch | patch -p1 )
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} ./configure --with-sdl-prefix=/usr/local \
		--prefix=/media/cryptofs/apps/usr/palm/applications/org.webosinternals.dosbox )
	( cd build/src ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
