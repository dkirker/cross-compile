NAME = luna-service
VERSION = 0.0.1
DEPENDS = common/dbus-lib

SRC_GIT = git://git.webos-internals.org/libraries/lunaservice.git

include ../../../support/download.mk
include ../../../support/staging.mk
include ../../../support/stage-rootfs.mk
include ../../../support/cross-compile.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -xf ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	touch $@

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( ${SB2} install -m644 build/src/lunaservice.h /usr/local/include/lunaservice.h  )
	( ${SB2} install -m644 build/src/lunaservice-errors.h /usr/local/include/lunaservice-errors.h  )
	touch $@

clobber::
	rm -rf build
