#
# debian package 'ctags' is required to build
#

NAME    = fishfillets
VERSION = 0.9.2
DEPENDS = libsdl libsdl-mixer libsdl-ttf libsdl-image lua
SRC_TGZ = http://prdownloads.sourceforge.net/fillets/fillets-ng-0.9.2.tar.gz?download
DATATGZ = http://prdownloads.sourceforge.net/fillets/fillets-ng-data-0.9.2.tar.gz?download
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.fishfillets

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

${DL_DIR}/${NAME}-${VERSION}.tar.gz:
	curl -f -R -L -o ${DL_DIR}/${NAME}-${VERSION}.tar.gz ${SRC_TGZ}

${DL_DIR}/${NAME}-data-${VERSION}.tar.gz:
	curl -f -R -L -o ${DL_DIR}/${NAME}-data-${VERSION}.tar.gz ${DATATGZ}

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz ${DL_DIR}/${NAME}-data-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/fillets-ng-${VERSION} build/src
	tar -C build -z -x -f ${DL_DIR}/${NAME}-data-${VERSION}.tar.gz
	mv build/fillets-ng-data-${VERSION} build/data
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/.patched:
	cp -r modified-${VERSION}/* build/src/src/
	touch $@

build/.configured:
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
	./configure --prefix=${APP_DIR} --disable-sdltest --disable-dependency-tracking --with-sdl-prefix=/usr/local )
	touch $@

build/.built: build/.unpacked build/.configured build/.patched
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	touch $@

build/%.built: build/.unpacked build/.configured build/.built
	mkdir -p ${APP_DIR}/share/games/fillets-ng
	cp -r build/data/* ${APP_DIR}/share/games/fillets-ng/
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} make clean )


clobber::
	rm -rf build
