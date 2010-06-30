NAME = sdlmame
VERSION = 0.138u2

SRC_TGZ = http://sdlmame4ubuntu.org/cur/urel/pool/unofficial/m/mame/mame_${VERSION}.orig.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	( cd build ; tar -zxvf ../$< )
	mv build/mame-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} $(MAKE) OSD=sdl TARGETOS=unix OPTIMIZE=0 NO_X11=1 NO_OPENGL=1 NOASM=1 USE_DISPATCH_GL=0 )
	touch $@

clobber::
	rm -rf build
