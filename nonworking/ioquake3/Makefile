NAME = ioquake3
VERSION = 1.36

SRC_GIT = git://git.webos-internals.org/games/ioquake3.git

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -xf ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	touch $@

include ../../support/staging.mk

stage::

include ../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src/src ; ${SB2} make \
		USE_WEBOS=1 BUILD_SERVER=0 \
		BUILD_SERVER=0 BUILD_CLIENT=1 BUILD_CLIENT_SMP=0 \
		BUILD_GAME_SO=1 BUILD_GAME_QVM=0 BUILD_STANDALONE=0 \
		USE_OPENAL=0 USE_OPENAL_DLOPEN=0 USE_CURL=0 USE_CURL_DLOPEN=0 \
		USE_CODEC_VORBIS=0 USE_LOCAL_HEADERS=0 USE_MUMBLE=0 USE_VOIP=0 \
	)
	touch $@

clobber::
	rm -rf build
