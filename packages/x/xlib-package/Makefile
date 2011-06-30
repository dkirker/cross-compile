# Packages up server and client X libraries.
NAME = xlib-package
VERSION = 0.3.0
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
	dev-gl/gles-headers    \
	common/adrenogfx       \
	common/tisgxgfx        \
	common/libpdl          \
	common/libsqlite       \
	x/libXaw               \
	x/libXi                \
	x/libXext              \
	x/libXft               \
	x/libXmu               \
	x/libXpm               \
	x/libXrender           \
	x/libxt                \
	x/libx11               \
	x/libice               \
	x/libsm

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

XLIBS= \
	/usr/local/lib/libICE.so.6             \
	/usr/local/lib/libSM.so.6              \
	/usr/local/lib/libX11-xcb.so.1         \
	/usr/local/lib/libX11.so.6             \
	/usr/local/lib/libXau.so.6             \
	/usr/local/lib/libXaw.so.6             \
	/usr/local/lib/libXaw.so.7             \
	/usr/local/lib/libXaw6.so.6            \
	/usr/local/lib/libXaw7.so.7            \
	/usr/local/lib/libXi.so.6              \
	/usr/local/lib/libXi.so.6.1.0          \
	/usr/local/lib/libXext.so.6            \
	/usr/local/lib/libXfont.so.1           \
	/usr/local/lib/libXft.so.2             \
	/usr/local/lib/libXmu.so.6             \
	/usr/local/lib/libXmuu.so.1            \
	/usr/local/lib/libXpm.so.4             \
	/usr/local/lib/libXrender.so.1         \
	/usr/local/lib/libXt.so.6              \
	/usr/local/lib/libfontenc.so.1         \
	/usr/local/lib/libpixman-1.so.0        \
	/usr/local/lib/libxcb-composite.so.0   \
	/usr/local/lib/libxcb-damage.so.0      \
	/usr/local/lib/libxcb-dpms.so.0        \
	/usr/local/lib/libxcb-dri2.so.0        \
	/usr/local/lib/libxcb-glx.so.0         \
	/usr/local/lib/libxcb-randr.so.0       \
	/usr/local/lib/libxcb-record.so.0      \
	/usr/local/lib/libxcb-render.so.0      \
	/usr/local/lib/libxcb-res.so.0         \
	/usr/local/lib/libxcb-screensaver.so.0 \
	/usr/local/lib/libxcb-shape.so.0       \
	/usr/local/lib/libxcb-shm.so.0         \
	/usr/local/lib/libxcb-sync.so.0        \
	/usr/local/lib/libxcb-xevie.so.0       \
	/usr/local/lib/libxcb-xf86dri.so.0     \
	/usr/local/lib/libxcb-xfixes.so.0      \
	/usr/local/lib/libxcb-xinerama.so.0    \
	/usr/local/lib/libxcb-xprint.so.0      \
	/usr/local/lib/libxcb-xtest.so.0       \
	/usr/local/lib/libxcb-xv.so.0          \
	/usr/local/lib/libxcb-xvmc.so.0        \
	/usr/local/lib/libxcb.so.1             \
	/usr/local/lib/libxkbfile.so.1

build/%.built:
	rm -f $@
	( rm -rf build/$* )
	( mkdir -p build/$* )
	( ${SB2} strip $(XLIBS) )
	( cd build/$* ; ${SB2} cp $(XLIBS) . )
	touch $@

clobber::
	rm -rf build

