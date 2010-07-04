NAME = mpg123
VERSION = 1.12.2
DEPENDS = common/glib-2.0

SRC_BZ2 = http://downloads.sourceforge.net/project/${NAME}/${NAME}/${VERSION}/${NAME}-${VERSION}.tar.bz2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS=\"-L${INSTALL_PREFIX}/lib -Wl,-rpath=${INSTALL_PREFIX}/lib\" ./configure \
		--prefix=${INSTALL_PREFIX} \
    )
	( cd build/src ; ${SB2} $(MAKE) && ${SB2} make install )
	touch $@

clobber::
	rm -rf build
