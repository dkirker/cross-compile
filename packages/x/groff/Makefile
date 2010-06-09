NAME = groff
VERSION = 1.20.1

DEPENDS =

SRC_TGZ = ftp://ftp.gnu.org/gnu/${NAME}/${NAME}-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage:: build

include ../../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	./configure \
		--prefix=${INSTALL_PREFIX} \
	)
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
