NAME = xorg-server
VERSION = 0.0.3
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
	common/tisgxgfx                           \
	common/libpdl                             \
	common/libsqlite

SRC_GIT = git://git.webos-internals.org/x11/xserver.git

#So this isn't the best, but here we hardcode X to
#expect to be in this directory.  This is where X looks all it's
#components and what-have-you, and helps avoid modifying
#the device's /usr/local
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xorg-server
#We add this to the -rpath so we have the libs X wants
XLIBS = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xlib

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

stage:: build

include ../../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( rm -rf build/obj )
	( mkdir -p build/obj )
	( cd build/obj ; ${SB2} \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${XLIBS}:.:/usr/local/lib\" \
	../src/configure \
		--prefix=${APP_DIR} \
		--enable-xsdl \
		--enable-kdrive \
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
	( cd build/obj ; ${SB2} $(MAKE))
	touch $@

clobber::
	rm -rf build
