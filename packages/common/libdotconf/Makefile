NAME = libdotconf
VERSION = 1.3
SRC_GIT = git://git.opentts.org/dotconf.git

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	rm -rf build
	mkdir build
	( cd build ; git clone -n ${SRC_GIT} src ; cd src ; git checkout v${VERSION} )
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/armv7.built
#	( cd ${STAGING_DIR}/usr/lib ; \
#     [ -e libgarmin.so ] || ln -s libgarmin.so.1.0.17 libgarmin.so )

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} aclocal )
	( cd build/$* ; ${SB2} autoheader )
	( cd build/$* ; ${SB2} autoconf )
	( cd build/$* ; ${SB2} libtoolize --auto$(MAKE) )
	( cd build/$* ; ${SB2} auto$(MAKE) --add-missing --copy --gnu )
	( cd build/$* ; ${SB2} export CFLAGS=-I${INSTALL_PREFIX}/include && \
		export LDFLAGS="-L${INSTALL_PREFIX}/lib -Wl,-rpath,${INSTALL_PREFIX}/lib" && \
 	        ${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/$* ; ${SB2} $(MAKE) install )
	mkdir -p build/$*/media/internal/.local/lib
	cp build/$*/src/.libs/libdotconf.so.0 build/$*/media/internal/.local/lib/
	touch $@

clobber::
	rm -rf build
