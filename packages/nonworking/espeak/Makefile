NAME    = espeak
VERSION = 1.43.21
#DEPENDS = pulseaudio
SVN_URL = https://espeak.svn.sourceforge.net/svnroot/espeak/trunk
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.espeak
REVISION= 241

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build
#	svn checkout -r ${REVISION} -q --non-interactive ${SVN_URL} build/src
	wget -O build/espeak-${VERSION}.zip http://espeak.sourceforge.net/test/espeak-${VERSION}.zip
	( cd build ; unzip espeak-${VERSION}.zip ; rm -rf src ; cp -a espeak-${VERSION} src )
	touch $@

update: build/.unpacked
#	( cd build/src ; \
#		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
#			svn update -r ${REVISION} --non-interactive; \
#			rm -f ../*.built ../.configure ../.aclocal; \
#		fi )

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built


build/.aclocal:
	rm -f $@
	touch $@

build/.patched:
#	$(foreach patchfile, $(shell ls patches), \
#		patch -p0 build/src/`echo ${patchfile} | sed 's#__#/#g'` < patches/$(patchfile); \
#	)
	touch $@

build/.configured: build/.aclocal
	rm -f $@
	touch $@


build/%.built: build/.unpacked build/.patched build/.configured
	( cd build/src/src ; ${SB2} make PREFIX=${APP_DIR} \
		AUDIO="" \
		CXXFLAGS="-O2" all)
	( ${SB2} mkdir -p build/${APP_DIR}/bin build/${APP_DIR}/lib build/${APP_DIR}/share )
	( ${SB2} cp -dR build/src/src/espeak build/${APP_DIR}/bin/ )
	( ${SB2} cp -dR build/src/src/libespeak.so.1* build/${APP_DIR}/lib/libespeak.so.1 )
	( ${SB2} cp -dR build/src/espeak-data build/${APP_DIR}/share/ )
	( ${SB2} cp -dR espeak build/${APP_DIR}/ )
	touch $@

clean:
	( cd build/src/src ; ${SB2} make clean )


clobber::
	rm -rf build
