#
# debian package 'ctags' is required to build
#

NAME    = stella
VERSION = 2.5
DEPENDS = jpeg libpng12 freetype fontconfig libsdl
#SRC_TGZ = http://downloads.sourceforge.net/project/stella/stella/2.2/stella-2.2-src.tar.gz
SRC_TGZ = http://sourceforge.net/projects/stella/files/stella/2.5/stella-2.5-src.tar.gz/download
APP_DIR = /media/cryptofs/apps/usr/palm/applications/de.daemon.stella

include ../../support/download.mk

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

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built


/usr/bin/ctags:
	@echo "*** debian package 'ctags' is required to build!   ***"
	@echo "*** install it as root with: apt-get install ctags ***"

build/.configured: /usr/bin/ctags
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" ./configure --prefix=${APP_DIR} \
		--enable-sound --enable-joystick )
	touch $@

build/%.built: build/.unpacked build/.configured
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} make clean )


clobber::
	rm -rf build
