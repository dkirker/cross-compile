NAME    := navit
DEPENDS := media/jpeg media/libpng12 common/glib-2.0 common/freetype common/fontconfig dev-sdl/libsdl dev-sdl/libsdl-image common/libpdl common/libgarmin common/locale apps/speechd
SRC_SVN_REV := http://navit.svn.sourceforge.net/svnroot/navit/trunk
APP_DIR := /media/cryptofs/apps/usr/palm/applications/org.webosinternals.navit
BINARY_GIT := git@git.webos-internals.org:/applications/navit.git
NAVIT_VERSION := `sed -nre "/AC_INIT\(navit,/ s/.* ([0-9\.]+).*\)/\1/ p" build/src/configure.in`
VERSION := 4113

include ../../../support/download.mk

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

.PHONY: unpack package package-commit clean

unpack: build/.unpacked
package: build/.package

build/.unpacked: ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz
	rm -f $@
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/navit build/src
	>$@

build/%.aclocal: build/.patched
	rm -f $@
	rm -rf build/$*
	mkdir -p build/$*
	cp -R build/src build/$*
	( cd build/$*/src ; ${SB2} ./autogen.sh )
	>$@

build/.patched: build/.unpacked
	rm -f $@
	$(foreach patchfile, $(shell ls patches), \
		patch -p2 -d build/src -i `pwd`/patches/$(patchfile); \
	)
	>$@

build/%.configured: build/${ARCH}.aclocal
	rm -f $@
	( cd build/$*/src ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./configure --prefix=${APP_DIR} \
			--disable-static --enable-buffer \
			--disable-gui-gtk --disable-graphics-opengl \
			--disable-binding-python --enable-avoid-float \
			--enable-avoid-unaligned --enable-debug \
			--disable-binding-dbus \
			--enable-speech-speech-dispatcher \
			--enable-vehicle-webos \
			--enable-svg2png-scaling="32,48" \
			--enable-svg2png-scaling-flag="32" \
			--enable-svg2png-scaling-nav="64" \
			--disable-samplemap --disable-speech-espeak \
			--disable-svg --with-svg2png-use-rsvg-convert \
			--enable-cache-size=20971520 )
	>$@


# I call $(MAKE) in xpm/ without sb2 here, it doesn't work inside sb2
# and creates images anyway
build/%.built: build/.unpacked build/.patched build/${ARCH}.configured
	rm -f $@
	( cd build/$*/src ; ${SB2} cp `which libtool` . )
	( cd build/$*/src/navit/xpm/ ; $(MAKE) -j8 )
	( cd build/$*/src ; ${SB2} $(MAKE) -j8 && ${SB2} make -j8 DESTDIR=`pwd`/.. install )
	find build/$*/${APP_DIR} -name "*.*a" -print0 | xargs -0 rm
	${SB2} gcc -I/usr/local/include/SDL -L/usr/local/lib -Wl,-rpath,/usr/local/lib,-rpath,`cd ../../../rootfs/${ARCH}/usr/lib; pwd` -lpdl files/pdl-helper.c -o build/$*/${APP_DIR}/pdl-helper
	mkdir -p build/$*/${APP_DIR}/dist_files/
	cp files/appinfo.json.in build/$*/${APP_DIR}/appinfo.json.in
#	cp files/jail_app.conf build/$*/${APP_DIR}/jail_app.conf
	cp files/Icon.png build/$*/${APP_DIR}/Icon.png
	cp files/navit.sh build/$*/${APP_DIR}/navit.sh
	cp files/navit*.xml build/$*/${APP_DIR}/dist_files/
	cp files/osd_*.png build/$*/${APP_DIR}/share/navit/xpm/
	cp -f files/poi_png/*.png build/$*/${APP_DIR}/share/navit/xpm/
	mkdir -p build/$*/${APP_DIR}/share/navit/xpm/skin/
	cp files/skin/*.png build/$*/${APP_DIR}/share/navit/xpm/skin/
	mkdir -p build/$*/${APP_DIR}/share/navit/xpm/PreNav/
	cp files/PreNav/*.png build/$*/${APP_DIR}/share/navit/xpm/PreNav/
	mkdir -p build/$*/${APP_DIR}/.fonts
	cp build/$*/src/navit/fonts/*.ttf build/$*/${APP_DIR}/.fonts/
	>$@

build/.package:
	rm -f $@
	make stage ARCH=armv7
	make stage ARCH=armv6
	mkdir -p build/package/all
	cp    build/armv7/${APP_DIR}/appinfo.json.in build/package/all
#	cp    build/armv7/${APP_DIR}/jail_app.conf build/package/all
	cp    build/armv7/${APP_DIR}/Icon.png build/package/all
	cp    build/armv7/${APP_DIR}/navit.sh build/package/all
	cp -R build/armv7/${APP_DIR}/dist_files build/package/all
	cp -R build/armv7/${APP_DIR}/share build/package/all
	cp -R build/armv7/${APP_DIR}/.fonts build/package/all
	mkdir -p build/package/armv7
	cp -R build/armv7/${APP_DIR}/pdl-helper build/package/armv7
	cp -R build/armv7/${APP_DIR}/bin build/package/armv7
	cp -R build/armv7/${APP_DIR}/lib build/package/armv7
	mkdir -p build/package/armv6
	cp -R build/armv6/${APP_DIR}/pdl-helper build/package/armv6
	cp -R build/armv6/${APP_DIR}/bin build/package/armv6
	cp -R build/armv6/${APP_DIR}/lib build/package/armv6
	cp -R files/control build/package/
	>$@


package-commit:
	if [ -d git ]; then (cd git && git checkout . && git pull); \
		else git clone ${BINARY_GIT} git;fi
	(cd git && rm -rf * )
	cp -a build/package/* git/
	(cd git && git add * && git commit -m "automatic revision" -a )

clean:
	( cd build/${ARCH} ; ${SB2} $(MAKE) clean )


clobber::
	rm -rf build
