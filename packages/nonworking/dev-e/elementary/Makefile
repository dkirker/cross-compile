NAME = elementary
VERSION = 0.6.0.063
DEPENDS = dev-e/edje dev-e/e_dbus dev-e/efreet

SVN_URL = http://svn.enlightenment.org/svn/e/trunk/TMP/st/${NAME}

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build/src
	svn checkout -q --non-interactive ${SVN_URL} build/src
	patch -d build/src -p0 < ${NAME}-webos-patch
	touch $@

update: build/.unpacked
	( cd build/src ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update --non-interactive; \
			rm -f ../*.built; \
		fi )

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ -O2" \
		LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
		PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
		./autogen.sh --prefix=${INSTALL_PREFIX} \
		--disable-static --disable-doc )
	( cd build/$* ; ${SB2} cp `which libtool` . )
	( cd build/$* ; ${SB2} $(MAKE) && ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
