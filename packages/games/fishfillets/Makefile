#
# debian package 'ctags' is required to build
#

NAME    = fishfillets
VERSION = 0.9.2
DEPENDS = dev-sdl/libsdl dev-sdl/libsdl-mixer dev-sdl/libsdl-ttf dev-sdl/libsdl-image dev-lang/lua
SRC_TGZ = http://prdownloads.sourceforge.net/fillets/fillets-ng-${VERSION}.tar.gz?download
DATATGZ = http://prdownloads.sourceforge.net/fillets/fillets-ng-data-${VERSION}.tar.gz?download
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.fishfillets

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/fillets-ng-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/armv7.built

build/.patched: build/.unpacked
	cp -r modified-${VERSION}/* build/src/src/
	touch $@

build/.configured: build/.patched
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
	./configure --prefix=${APP_DIR} --datadir=${APP_DIR}/data --disable-sdltest --disable-dependency-tracking --with-sdl-prefix=/usr/local )
	touch $@

build/%.built: build/.configured
	( cd build/src ; ${SB2} cp `which libtool` . )
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} $(MAKE) clean )


clobber::
	rm -rf build







# developer specific, for ipk build only, runs outside staging builds
fishinstall: build/.fishinstall build/.fishdatainstall

build/.fishinstall: build/armv7.built 
	( cd build/src ; ${SB2} $(MAKE) && ${SB2} make install )
	touch $@

build/.fishdatadownload:
	curl -f -R -L -o data.tar.gz ${DATATGZ}
	touch $@

build/.fishdata: build/.fishdatadownload
	tar -C build -z -x -f data.tar.gz
	mv build/fillets-ng-data-${VERSION} build/data
	touch $@

build/.fishdatainstall: build/.fishdata
	cp -r build/data ${APP_DIR}/
	touch $@
