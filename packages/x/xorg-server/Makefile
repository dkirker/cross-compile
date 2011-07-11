NAME = xorg-server
VERSION := 0.3.1

DEPENDS = \
	common/openssl         \
	dev-sdl/libsdl         \
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
	x/recordproto          \
	x/scrnsaverproto       \
	x/videoproto           \
	x/xcmiscproto          \
	x/xextproto            \
	x/xineramaproto        \
	x/xproto               \
	x/xtrans               \
	x/xau                  \
	x/xkbfile              \
	x/xfont                \
	x/libxcb               \
	x/pixman               \
	dev-gl/gles-headers                       \
	common/adrenogfx			  \
	common/tisgxgfx                           \
	common/libpdl                             \
	common/libsqlite

SRC_GIT = git://git.webos-internals.org/x11/xserver.git

ifndef XAPPID
XAPPID = xorg-server
endif

APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.$(XAPPID)

#We add this to the -rpath so we have the libs X wants
XLIBS = $(APP_DIR)/xlib

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build src
	tar -C src -z -x -f $<
	mv src build/src
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}-$(XAPPID).built

build/%.built: build/.unpacked
	rm -f $@
	( rm -rf build/$* )
	( mkdir -p build/$* )
	( cd build/$* ; ${SB2} \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib -Wl,--allow-shlib-undefined\" \
	../src/configure \
		--prefix=${APP_DIR} \
		--enable-kdrive \
		--enable-xsdl \
		\
		--disable-xorg \
		--disable-glx \
		--disable-xquartz \
		--disable-xnest \
		--disable-xwin \
		--disable-xfake \
		--disable-xfbdev \
		--disable-xvfb \
		--disable-dri \
	)
	( ${SB2} gcc -o build/$*/forker forker.c )
	[ "${ARCH}" != "armv6" ] || ( cd build/$*/hw/kdrive/sdl ; sed -i "s/^LIBS = /LIBS = -legl13 /" Makefile )
	( cd build/$* ; ${SB2} $(MAKE))
	touch $@

rebuild:
	( cd build/armv7-xserver ; ${SB2} $(MAKE))

clobber::
	rm -rf build
