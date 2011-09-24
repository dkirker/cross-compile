NAME    = espeak
VERSION = 1.45.03
SRC_ZIP = http://downloads.sourceforge.net/project/espeak/espeak/espeak-1.45/espeak-1.45.03-source.zip
#SRC_SVN = http://espeak.svn.sourceforge.net/svnroot/espeak/trunk
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.speechd
#REVISION= 252
SO_VER  = 1.1.45

include ../../../support/download.mk

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

.PHONY: unpack package
unpack: build/.unpacked
package: build/.package

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.zip
	mkdir -p build
	unzip -o -d build $<
	rm -rf build/src
	mv build/espeak-${VERSION}-source build/src
	touch $@

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/src build/$*
	( cd build/$*/src ; ${SB2} $(MAKE) PREFIX=${APP_DIR} \
		AUDIO="" all)
	mkdir -p build/$*/${APP_DIR}/bin build/$*/${APP_DIR}/lib build/$*/${APP_DIR}/share
	cp -dR build/$*/src/speak build/$*/${APP_DIR}/bin/
	cp -dR build/$*/src/espeak build/$*/${APP_DIR}/bin/
	cp -dR build/$*/src/libespeak.so.${SO_VER} build/$*/${APP_DIR}/lib/libespeak.so.1
	cp -dR build/src/espeak-data build/$*/${APP_DIR}/share/
	cp -dR files/* build/$*/${APP_DIR}/
	( ${SB2} mkdir -p ${INSTALL_PREFIX}/lib )
	( ${SB2} rm -f  ${INSTALL_PREFIX}/lib/libespeak.* )
	( ${SB2} cp -dR build/$*/src/libespeak.so* ${INSTALL_PREFIX}/lib )
	( ${SB2} ln -fs libespeak.so.${SO_VER} ${INSTALL_PREFIX}/lib/libespeak.so )
	( ${SB2} rm -rf ${INSTALL_PREFIX}/include/espeak )
	( ${SB2} mkdir -p ${INSTALL_PREFIX}/include/espeak )
	( ${SB2} cp -dR build/$*/src/speak_lib.h ${INSTALL_PREFIX}/include/espeak )
	touch $@

build/.package:
	rm -f $@
	make stage ARCH=armv7
	#make stage ARCH=armv6
	mkdir -p build/package/all
	cp -R build/armv7/${APP_DIR}/share build/package/all
	mkdir -p build/package/armv7
	cp -R build/armv7/${APP_DIR}/bin build/package/armv7
	cp -R build/armv7/${APP_DIR}/lib build/package/armv7
	#mkdir -p build/package/armv6
	#cp -R build/armv6/${APP_DIR}/bin build/package/armv6
	#cp -R build/armv6/${APP_DIR}/lib build/package/armv6
	touch $@

clean:
	( cd build/${ARCH}/src ; ${SB2} $(MAKE) clean )

clobber::
	rm -rf build
