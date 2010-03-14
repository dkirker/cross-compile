NAME = xorg-server
VERSION = 0.0.3
DEPENDS = libsdl openssl \
	../nonworking/bigreqsproto         \
	../nonworking/compositeproto       \
	../nonworking/damageproto          \
	../nonworking/fixesproto           \
	../nonworking/fontsproto           \
	../nonworking/inputproto           \
	../nonworking/kbproto              \
	../nonworking/randrproto           \
	../nonworking/renderproto          \
	../nonworking/resourceproto        \
	../nonworking/scrnsaverproto       \
	../nonworking/videoproto           \
	../nonworking/xcmiscproto          \
	../nonworking/xextproto            \
	../nonworking/xineramaproto        \
	../nonworking/xproto               \
	../nonworking/xtrans               \
	../nonworking/xau                  \
	../nonworking/xkbfile              \
	../nonworking/xfont                \
	../nonworking/libxcb               \
	../nonworking/pixman               \
	gles-headers                       \
	tisgxgfx                           \
	libpdl                             \
	libsqlite

SRC_GIT = git://git.webos-internals.org/x11/xserver.git

#So this isn't the best, but here we hardcode X to
#expect to be in this directory.  This is where X looks all it's
#components and what-have-you, and helps avoid modifying
#the device's /usr/local
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.xlauncher

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build src
	tar -C src -z -x -f $<
	mv src build/src
	touch $@

include ../../support/staging.mk

stage:: build

include ../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( rm -rf build/obj )
	( mkdir -p build/obj )
	( cd build/obj ; ${SB2} \
	LDFLAGS=\"-L/usr/local/lib -Wl,-rpath=${APP_DIR}/lib:.:/usr/local/lib\" \
	PKG_CONFIG_LIBDIR=/usr/local/lib/pkgconfig:/usr/local/share/pkgconfig \
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
	( cd build/obj ; ${SB2} make)
	touch $@

clobber::
	rm -rf build
