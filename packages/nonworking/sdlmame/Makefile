NAME = sdlmame
VERSION = 0136

SRC_ZIP = http://rbelmont.mameworld.info/sdlmame0136.zip

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.zip
	rm -rf build
	mkdir -p build
	( cd build ; unzip ../$< )
	mv build/${NAME}${VERSION} build/src
	touch $@

include ../../support/staging.mk

stage::

include ../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} $(MAKE) OSD=sdl TARGETOS=unix OPTIMIZE=0 NO_X11=1 NO_OPENGL=1 NOASM=1 USE_DISPATCH_GL=0 )
	touch $@

clobber::
	rm -rf build
