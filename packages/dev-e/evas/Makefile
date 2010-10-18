NAME = evas
VERSION = 0.9.9.063
DEPENDS = media/jpeg media/libpng12 common/freetype common/fontconfig dev-sdl/libsdl dev-e/eina dev-e/eet dev-gl/gles-headers common/libxml2

SVN_URL = http://svn.enlightenment.org/svn/e/trunk/${NAME}

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build/src
	svn checkout -q --non-interactive ${SVN_URL} build/src
	touch $@

build/.patched: build/.unpacked
	touch $@

update: build/.patched
	( cd build/src ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update --non-interactive; \
			rm -f ../*.built; \
		fi )

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/$(ARCH).built

build/%.built: build/.patched
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./autogen.sh --prefix=${INSTALL_PREFIX} \
		--disable-static --enable-buffer \
		--enable-software-sdl --enable-fb \
		--enable-gl-sdl --enable-gl-flavor-gles \
		--enable-gles-variety-sgx --disable-doc \
		--disable-fribidi --disable-image-loader-svg )
	( cd build/$* ; ${SB2} cp `which libtool` . )
	( cd build/$* ; ${SB2} $(MAKE) && ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
