NAME    = gnash
VERSION = 0.8.7
DEPENDS = media/jpeg media/libpng12 media/freetype media/fontconfig dev-sdl/libsdl media/agg media/ffmpeg
SRC_TGZ = http://ftp.gnu.org/pub/gnu/gnash/0.8.7/gnash-0.8.7.tar.gz
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.gnash

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

${DL_DIR}/${NAME}-${VERSION}-src.tar.gz:
	curl -f -R -L -o ${DL_DIR}/${NAME}-${VERSION}-src.tar.gz ${SRC_TGZ}

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}-src.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built


build/.configured:
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
		./configure --prefix=${APP_DIR} \
		--enable-renderer=AGG --enable-media=ffmpeg --enable-gui=fb --disable-testsuite \
		--disable-menus --disable-swftree --enable-sound=sdl --disable-plugins \
		--disable-docbook )
	touch $@

build/%.built: build/.unpacked build/.configured
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src ; ${SB2} $(MAKE) && ${SB2} make install )
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} $(MAKE) clean )


clobber::
	rm -rf build
