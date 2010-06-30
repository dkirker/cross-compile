#
# debian package 'ctags' is required to build
#

NAME    = stella
VERSION = 3.0
DEPENDS = jpeg libpng12 freetype fontconfig libsdl
SRC_TGZ = http://sourceforge.net/projects/stella/files/stella/${VESION}/stella-${VERSION}-src.tar.gz/download
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webos-internals.stella

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

stage-local:: build/${ARCH}.built


/usr/bin/ctags:
	@echo "*** debian package 'ctags' is required to build!   ***"
	@echo "*** install it as root with: apt-get install ctags ***"

build/.configured: /usr/bin/ctags
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" ./configure --prefix=${APP_DIR} \
		--disable-debugger --enable-sound --disable-joystick )
	touch $@

patchsource:
	cp OSystemUNIX.cxx     build/src/src/unix/OSystemUNIX.cxx
	cp EventHandler.cxx    build/src/src/emucore/EventHandler.cxx
	cp EditTextWidget.cxx  build/src/src/gui/EditTextWidget.cxx

build/%.built: build/.unpacked build/.configured patchsource
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src ; ${SB2} $(MAKE) && ${SB2} make install )
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} $(MAKE) clean )


clobber::
	rm -rf build
