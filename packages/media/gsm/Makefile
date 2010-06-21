NAME = gsm
VERSION = 1.0.12
DEPENDS = 

SRC_TGZ = http://www.quut.com/${NAME}/${NAME}-${VERSION}.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-1.0-pl12 build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	${SB2} rm -f /usr/local/lib/libgsm.a /usr/local/include/gsm.h
	( cd build/src ; ${SB2} $(MAKE) INSTALL_ROOT=/usr/local GSM_INSTALL_INC=/usr/local/include gsminstall)
	touch $@

clobber::
	rm -rf build
