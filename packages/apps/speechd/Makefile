NAME = speechd
VERSION = 0.7
DEPENDS = common/locale common/libdotconf media/libsndfile media/pulseaudio
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.${NAME}

SPD_SRC_GIT = git://git.freebsoft.org/git/speechd

ESP_ZIP_FILE = espeak-1.43.54.zip
#ESP_SRC_ZIP = http://espeak.sourceforge.net/test/${ESP_ZIP_FILE}

.PHONY: unpack
unpack: build/.unpacked

include ../../../support/staging.mk
include ../../../support/cross-compile.mk
include ../../../support/download.mk

stage-local:: build/${ARCH}.built

build/.downloaded: ${DL_DIR}/${ESP_ZIP_FILE}

${DL_DIR}/${ESP_ZIP_FILE}:
	cp files/${ESP_ZIP_FILE} ${DL_DIR}/

build/.unpacked: build/.downloaded
	rm -f $@
	rm -rf build
	mkdir build
	( cd build ; git clone -n ${SPD_SRC_GIT} src ; cd src ; git checkout ${VERSION} )
	mkdir build/espeak
	( cd build/espeak; unzip ../../files/${ESP_ZIP_FILE} )
	mv build/espeak/*/* build/espeak/
	touch $@

build/.patched: build/.unpacked
	rm -f $@
	$(foreach patchfile, $(shell ls patches), \
		patch -d build/src -p1 < patches/$(patchfile); \
	)
	touch $@

build/%.built: build/.unpacked build/.patched
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$* #Really guys? A package within a package?
	mkdir -p build/espeak/$*
	cp -R build/espeak/src/* build/espeak/$*
	( cd build/espeak/$* ; ${SB2} $(MAKE) PREFIX=${APP_DIR} AUDIO="" all )
	${SB2} mkdir -p ${INSTALL_PREFIX}/include/espeak
	${SB2} cp -dR build/espeak/$*/libespeak.so* build/espeak/$*/libespeak.a ${INSTALL_PREFIX}/lib/
	${SB2} cp -dR build/espeak/$*/speak_lib.h ${INSTALL_PREFIX}/include/espeak/
	mkdir -p build/$*/${APP_DIR}/bin build/$*/${APP_DIR}/lib build/$*/${APP_DIR}/share
	cp build/espeak/$*/speak build/$*/${APP_DIR}/bin/
	cp build/espeak/$*/espeak build/$*/${APP_DIR}/bin/
	cp build/espeak/$*/libespeak.so.1.* build/$*/${APP_DIR}/lib/libespeak.so.1
	cp -dR build/espeak/espeak-data build/$*/${APP_DIR}/share/
	( cd build/$* ; ${SB2} ./build.sh )
	( cd build/$* ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./configure --prefix=${APP_DIR} )
	( cd build/$* ; ${SB2} $(MAKE) )
	( cd build/$* ; ${SB2} $(MAKE) DESTDIR=`pwd` install )
	${SB2} cp -dR build/$*/src/c/api/libspeechd.h ${INSTALL_PREFIX}/include/
	${SB2} cp -dR build/$*/src/c/api/.libs/*.so* ${INSTALL_PREFIX}/lib/
	${SB2} cp ${INSTALL_PREFIX}/lib/libdotconf.so.0 build/$*/${APP_DIR}/lib/libdotconf.so.0
	mv build/$*/${APP_DIR}/lib/libspeechd.so.2.* build/$*/${APP_DIR}/lib/libspeechd.so.2
	mv build/$*/${APP_DIR}/lib/speech-dispatcher/libsdaudio.so.2.* \
		build/$*/${APP_DIR}/lib/speech-dispatcher/libsdaudio.so.2
	rm -f build/$*/${APP_DIR}/lib/libspeechd.so
	rm -f build/$*/${APP_DIR}/lib/libspeechd.*a
	rm -r build/$*/${APP_DIR}/include
	cp files/{speech-dispatcher,spd-say} build/$*/${APP_DIR}
	cp files/speechd.conf build/$*/${APP_DIR}/etc/speech-dispatcher/
	cp files/espeak-generic.conf build/$*/${APP_DIR}/etc/speech-dispatcher/modules/
	touch $@

clobber::
	rm -rf build
