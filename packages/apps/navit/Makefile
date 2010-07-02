#
# How to make localization work (maybe):
#
# (WARNING, this involves changes to the root-partition of your device!! You need to be very careful!)
# 
# The C-lib expects a locale-archive in /usr/lib/locale/locale-archive.
# This path is hardcoded. I don't know how to generate one, but for me the following works:
#
# - create dir /media/cryptofs/apps/usr/lib/locale/
# - make / writable, goto /usr/lib
# - create symlink to /media/cryptofs/apps/usr/lib/locale/, make / read-only again
# - copy the locale-archive of your developer machine to /media/cryptofs/apps/usr/lib/locale/
#
# You need to adjust the LANG-variable in navit.sh
#
# Good luck ...
#

NAME    = navit
VERSION = 0.2.0
DEPENDS = media/jpeg media/libpng12 common/freetype common/fontconfig dev-sdl/libsdl dev-sdl/libsdl-image common/libpdl common/libgarmin
SVN_URL = http://navit.svn.sourceforge.net/svnroot/navit/trunk
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.navit
REVISION= 3471

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build
	svn checkout -r ${REVISION} -q --non-interactive ${SVN_URL} build
	mv build/navit build/src
	touch $@

update: build/.unpacked
	( cd build/$* ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update -r ${REVISION} --non-interactive; \
			rm -f ../*.built ../.configure ../.aclocal; \
		fi )

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built


build/%.aclocal: build/.patched
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} ./autogen.sh )
	touch $@

build/.patched: build/.unpacked
	$(foreach patchfile, $(shell ls patches), \
		patch -p0 build/src/`echo ${patchfile} | sed 's#__#/#g'` < patches/$(patchfile); \
	)
	touch $@

build/%.configured: build/${ARCH}.aclocal
	rm -f $@
	( cd build/$* ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
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
                --enable-cache-size=10485760 )
	touch $@


# I call $(MAKE) in xpm/ without sb2 here, it doesn't work inside sb2
# and creates images anyway
build/%.built: build/.unpacked build/.patched build/${ARCH}.configured
	rm -f $@
	( cd build/$* ; ${SB2} cp `which libtool` . )
	( cd build/$*/navit/xpm/ ; $(MAKE) )
	( cd build/$* ; ${SB2} $(MAKE) && ${SB2} make DESTDIR=`cd .. ; pwd` install )
	${SB2} mkdir -p build/${APP_DIR}/dist_files/
	( ${SB2} gcc -I/usr/local/include/SDL -L/usr/local/lib -Wl,-rpath,/usr/local/lib,-rpath,`cd ../../../rootfs/armv7/usr/lib; pwd` -lpdl files/pdl-helper.c -o build/${APP_DIR}/pdl-helper )
	${SB2} cp files/appinfo.json build/${APP_DIR}/appinfo.json
	${SB2} cp files/Icon.png build/${APP_DIR}/Icon.png
	${SB2} cp files/navit.sh build/${APP_DIR}/navit.sh
	${SB2} cp files/navit*.xml build/${APP_DIR}/dist_files/
	${SB2} cp files/osd_*.png build/${APP_DIR}/share/navit/xpm/
	${SB2} cp -f files/poi_png/*.png build/${APP_DIR}/share/navit/xpm/
	${SB2} mkdir -p build/${APP_DIR}/share/navit/xpm/skin/
	${SB2} cp files/skin/*.png build/${APP_DIR}/share/navit/xpm/skin/
	${SB2} mkdir -p build/${APP_DIR}/share/navit/xpm/PreNav/
	${SB2} cp files/PreNav/*.png build/${APP_DIR}/share/navit/xpm/PreNav/
	touch $@

clean:
	( cd build/${ARCH} ; ${SB2} $(MAKE) clean )


clobber::
	rm -rf build
