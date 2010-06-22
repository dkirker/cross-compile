NAME = libX11
VERSION = 1.3.2

DEPENDS = \
	x/bigreqsproto         \
	x/compositeproto       \
	x/damageproto          \
	x/fixesproto           \
	x/fontsproto           \
	x/inputproto           \
	x/kbproto              \
	x/randrproto           \
	x/renderproto          \
	x/resourceproto        \
	x/scrnsaverproto       \
	x/videoproto           \
	x/xcmiscproto          \
	x/xextproto            \
	x/xineramaproto        \
	x/xproto               \
	x/xtrans               \
	x/xau                  \
	x/libxcb
	

SRC_BZ2 = http://www.x.org/releases/X11R7.5/src/everything/${NAME}-${VERSION}.tar.bz2

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

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--disable-specs \
	)
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
