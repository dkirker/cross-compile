NAME = xkeyboard-config
VERSION = 1.7-2

DEPENDS=x/xkbcomp

SRC_GIT=git://git.webos-internals.org/x11/xkeyboard-config.git

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -z -x -f $<
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	( cd build/src ; ${SB2} \
		./configure \
		--prefix=${INSTALL_PREFIX} \
	)
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
