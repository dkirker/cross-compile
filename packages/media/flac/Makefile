NAME = flac
VERSION = 1.2.1
DEPENDS = common/glib-2.0 media/libogg

SRC_TGZ = http://downloads.sourceforge.net/project/${NAME}/${NAME}-src/${NAME}-${VERSION}-src/${NAME}-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS=\"-L${INSTALL_PREFIX}/lib -Wl,-rpath=${INSTALL_PREFIX}/lib\" ./configure \
		--prefix=${INSTALL_PREFIX} \
		--disable-xmms-plugin \
    )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	touch $@

clobber::
	rm -rf build
