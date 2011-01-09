NAME = qt
VERSION = 4.7
DEPENDS = dev-sdl/libsdl x/libx11 x/libXext x/libsm x/libice media/pulseaudio media/libpng12 media/jpeg media/alsa-lib

SRC_GIT = git://gitorious.org/~darronb/qt/qt-palm-pre.git
GIT_BRANCH = ${VERSION}-stable

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/qt-palm-pre
	tar -C build/qt-palm-pre -xzf $<
	mv build/qt-palm-pre build/src
	( cd build/src ; patch -p0 < ../../fix_race_cond.patch )
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} PATH=${PATH}:/srv/preware/cross-compile/packages/common/qt/build/armv7/bin/ PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ ./configure -embedded -opensource -plugin-gfx-webos -no-accessibility -no-qt3support -no-gif -no-libtiff -no-libmng -no-webkit -no-phonon -no-phonon-backend -no-cups -no-nis -no-iconv -no-dbus -nomake tools -nomake examples -nomake docs -nomake demos -v -L/usr/local/lib -R/usr/local/lib )
	( cd build/$* ; ${SB2} PATH=${PATH}:/srv/preware/cross-compile/packages/common/qt/build/armv7/bin/ $(MAKE) )
	( cd build/$* ; ${SB2} PATH=${PATH}:/srv/preware/cross-compile/packages/common/qt/build/armv7/bin/ $(MAKE) install )
	touch $@

clobber::
	rm -rf build
