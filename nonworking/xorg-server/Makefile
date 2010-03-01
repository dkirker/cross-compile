NAME = xorg-server
VERSION = 1.7.1
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
	../nonworking/pixman

SRC_BZ2 = http://www.x.org/releases/X11R7.5/src/everything/${NAME}-${VERSION}.tar.bz2

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
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
		--disable-dri \
		--enable-xsdl \
		--enable-kdrive \
		--disable-xorg \
		--disable-glx \
		--disable-xquartz \
		--disable-xnest \
		--disable-xwin \
		--disable-xfake \
		--disable-aiglx \
		--disable-config-hal \
		--enable-xfbdev \
		--disable-dpms \
		--disable-xaa \
	)
	( cd build/src ; ${SB2} make)
	touch $@

clobber::
	rm -rf build
