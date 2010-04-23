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
DEPENDS = media/jpeg media/libpng12 common/freetype common/fontconfig dev-sdl/libsdl dev-sdl/libsdl-image nonworking/pdl
SVN_URL = https://navit.svn.sourceforge.net/svnroot/navit/trunk
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.navit
REVISION= 3191

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build
	svn checkout -r ${REVISION} -q --non-interactive ${SVN_URL} build
	mv build/navit build/src
	touch $@

update: build/.unpacked
	( cd build/src ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update -r ${REVISION} --non-interactive; \
			rm -f ../*.built ../.configure ../.aclocal; \
		fi )

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built


build/.aclocal:
	rm -f $@
	( cd build/src ; ${SB2} ./autogen.sh )
	touch $@

build/.patched:
	$(foreach patchfile, $(shell ls patches), \
		patch -p0 build/src/`echo ${patchfile} | sed 's#__#/#g'` < patches/$(patchfile); \
	)
	touch $@

build/.configured: build/.aclocal
	rm -f $@
	( cd build/src ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./configure --prefix=${APP_DIR} \
		--disable-static --enable-buffer \
		--disable-gui-gtk --disable-graphics-opengl \
		--disable-binding-python --enable-avoid-float \
		--enable-avoid-unaligned --enable-debug \
		--disable-binding-dbus \
		--enable-vehicle-webos \
		--enable-svg2png-scaling="32,48,64" \
		--enable-svg2png-scaling-flag="32,48,64" \
		--enable-svg2png-scaling-nav="8,16,32,48,64" \
		--disable-samplemap --disable-speech-espeak \
		--disable-svg --with-svg2png-use-rsvg-convert )
	touch $@


# I call make in xpm/ without sb2 here, it doesn't work inside sb2
# and creates images anyway
build/%.built: build/.unpacked build/.patched build/.configured
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src/navit/xpm/ ; make )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	${SB2} mkdir -p ${APP_DIR}/dist_files/
	${SB2} cp files/appinfo.json ${APP_DIR}/appinfo.json
	${SB2} cp files/Icon.png ${APP_DIR}/Icon.png
	${SB2} cp files/navit.sh ${APP_DIR}/navit.sh
	${SB2} cp files/navit*.xml ${APP_DIR}/dist_files/
	${SB2} cp files/osd_*.png ${APP_DIR}/share/navit/xpm/
	touch $@

clean:
	( cd build/src ; ${SB2} make clean )


clobber::
	rm -rf build
