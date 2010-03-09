NAME = xorg-server
VERSION = 0.0.1alpha
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
	gles-headers

SRC_GIT = git://git.webos-internals.org/x11/xserver.git

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
	( cd build/src ; ${SB2} \
	PKG_CONFIG_LIBDIR=/usr/local/lib/pkgconfig:/usr/local/share/pkgconfig \
	./configure \
		--prefix=${INSTALL_PREFIX} \
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
	( cd build/src ; ${SB2} make)
	touch $@

clobber::
	rm -rf build
