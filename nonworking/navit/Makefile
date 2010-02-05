NAME    = navit
VERSION = 0.1.1
DEPENDS = jpeg libpng12 freetype fontconfig libsdl
SVN_URL = https://navit.svn.sourceforge.net/svnroot/navit/trunk
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webos-internals.navit


.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build/src
	svn checkout -q --non-interactive ${SVN_URL} build/src
	if [ ! -e  build/src/${NAME}/navit/xpm/mark.xpm -a ! -e build/src/${NAME}/navit/xpm/mark.svg ]; then \
		cp mark* build/src/${NAME}/navit/xpm/ \
	fi
	touch $@

update: build/.unpacked
	( cd build/src ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update --non-interactive; \
			rm -f ../*.built ../.configure ../.aclocal; \
		fi )
	if [ ! -e  build/src/${NAME}/navit/xpm/mark.xpm -a ! -e build/src/${NAME}/navit/xpm/mark.svg ]; then \
		cp mark* build/src/${NAME}/navit/xpm/ \
	fi

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
		--enable-svg2png-scaling="32,48,64" --enable-svg2png-scaling-flag="32,48,64" \
		--enable-svg2png-scaling-nav="8,16,32,48,64" --disable-samplemap \
		--enable-vehicle-gpsd --enable-speech-espeak \
		--enable-binding-dbus \
		--with-svg2png-use-convert )
	touch $@

build/%.built: build/.unpacked build/.configured
	( cd build/src/${NAME} ; ${SB2} cp `which libtool` . )
	( cd build/src/${NAME} ; ${SB2} make && ${SB2} make install )
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} make clean )


clobber::
	rm -rf build
