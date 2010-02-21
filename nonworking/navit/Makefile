NAME    = navit
VERSION = 0.1.1
DEPENDS = jpeg libpng12 freetype fontconfig libsdl dbus-lib 
SVN_URL = https://navit.svn.sourceforge.net/svnroot/navit/trunk
APP_DIR = /media/cryptofs/apps/usr/palm/applications/de.daemon.navit
REVISION= 2914

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build/src
	svn checkout -r ${REVISION} -q --non-interactive ${SVN_URL} build/src
	touch $@

update: build/.unpacked
	( cd build/src ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update -r ${REVISION} --non-interactive; \
			rm -f ../*.built ../.configure ../.aclocal; \
		fi )
	cp graphics_sdl.c ./build/src/navit/navit/graphics/sdl/graphics_sdl.c

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built


build/.aclocal:
	rm -f $@
	( cd build/src/${NAME} ; ${SB2} ./autogen.sh )
	touch $@
	


build/.configured: build/.aclocal
	rm -f $@
	( cd build/src/${NAME} ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./configure --prefix=${APP_DIR} \
		--disable-static --enable-buffer \
		--disable-gui-gtk --disable-graphics-opengl \
		--disable-binding-python --enable-avoid-float \
		--enable-avoid-unaligned --enable-debug \
		--disable-binding-dbus \
		--enable-svg2png-scaling="32,48,64" --enable-svg2png-scaling-flag="32,48,64" \
		--enable-svg2png-scaling-nav="8,16,32,48,64" --disable-samplemap \
		--disable-speech-espeak \
		--with-svg2png-use-convert )
	touch $@


# I call make in xpm/ without sb2 here, it doesn't work inside sb2
# and creates images anyway
build/%.built: build/.unpacked build/.configured
	( cd build/src/${NAME} ; ${SB2} cp `which libtool` . )
	( cd build/src/${NAME}/navit/xpm/ ; make )
	( cd build/src/${NAME} ; ${SB2} make && ${SB2} make install )
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} make clean )


clobber::
	rm -rf build
