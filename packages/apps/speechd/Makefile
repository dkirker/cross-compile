NAME = speechd
VERSION = 0.7
DEPENDS = common/locale common/libdotconf media/libsndfile media/pulseaudio apps/espeak
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.${NAME}
APP_PREFIX= /var/usr
SRC_TGZ = http://www.freebsoft.org/pub/projects/speechd/speech-dispatcher-${VERSION}.tar.gz
ESPEAK = ../espeak

include ../../../support/download.mk

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

.PHONY: unpack package
unpack: build/.unpacked
package: build/.package

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -f $@
	mkdir -p build
	tar -C build -z -x -f $<
	rm -rf build/src
	mv build/speech-dispatcher-${VERSION} build/src
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
	cp -R build/src build/$*
	( cd build/$*/src ; ${SB2} ./build.sh )
	( cd build/$*/src ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./configure --prefix=${APP_PREFIX} )
	( cd build/$*/src ; ${SB2} $(MAKE) )
	( cd build/$*/src ; ${SB2} $(MAKE) DESTDIR=`pwd`/.. install )
	${SB2} cp -dR build/$*/src/src/c/api/libspeechd.h ${INSTALL_PREFIX}/include/
	${SB2} cp -dR build/$*/src/src/c/api/.libs/*.so* ${INSTALL_PREFIX}/lib/
	mkdir -p build/$*/${APP_DIR}
	cp -R build/$*/${APP_PREFIX}/* build/$*/${APP_DIR}
	${SB2} cp ${INSTALL_PREFIX}/lib/libdotconf.so.0 build/$*/${APP_DIR}/lib/libdotconf.so.0
	mv build/$*/${APP_PREFIX}/lib/libspeechd.so.2.* build/$*/${APP_DIR}/lib/libspeechd.so.2
	mv build/$*/${APP_PREFIX}/lib/speech-dispatcher/libsdaudio.so.2.* \
		build/$*/${APP_DIR}/lib/speech-dispatcher/libsdaudio.so.2
	rm -f build/$*/${APP_PREFIX}/lib/libspeechd.so
	rm -f build/$*/${APP_PREFIX}/lib/libspeechd.*a
	rm -f build/$*/${APP_PREFIX}/lib/speech-dispatcher/libsdaudio.so
	rm -f build/$*/${APP_PREFIX}/lib/speech-dispatcher/libsdaudio.*a
	rm -r build/$*/${APP_PREFIX}/include
	cp files/{speech-dispatcher,spd-say} build/$*/${APP_DIR}
	cp files/speechd.conf build/$*/${APP_DIR}/etc/speech-dispatcher/
	cp files/espeak-generic.conf build/$*/${APP_DIR}/etc/speech-dispatcher/modules/
	touch $@

build/.package:
	rm -f $@
	make stage ARCH=armv7
	make stage ARCH=armv6
	mkdir -p build/package/all
	cp    build/armv7/${APP_DIR}/spd-say build/package/all
	cp    build/armv7/${APP_DIR}/speech-dispatcher build/package/all
	cp -R build/armv7/${APP_DIR}/etc build/package/all
	cp -R build/armv7/${APP_DIR}/share build/package/all
	mkdir -p build/package/armv7
	cp -R build/armv7/${APP_DIR}/bin build/package/armv7
	cp -R build/armv7/${APP_DIR}/lib build/package/armv7
	mkdir -p build/package/armv6
	cp -R build/armv6/${APP_DIR}/bin build/package/armv6
	cp -R build/armv6/${APP_DIR}/lib build/package/armv6
	# pull in the espeak-stuff
	cp -R ${ESPEAK}/build/armv7/${APP_DIR}/share build/package/all
	cp -R ${ESPEAK}/build/armv7/${APP_DIR}/bin build/package/armv7
	cp -R ${ESPEAK}/build/armv7/${APP_DIR}/lib build/package/armv7
	cp -R ${ESPEAK}/build/armv6/${APP_DIR}/bin build/package/armv6
	cp -R ${ESPEAK}/build/armv6/${APP_DIR}/lib build/package/armv6
	touch $@

clobber::
	rm -rf build
