NAME = gsm
VERSION = 1.0.12
DEPENDS = 

SRC_TGZ = http://www.quut.com/${NAME}/${NAME}-${VERSION}.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-1.0-pl12 build/src
	cp build/src/Makefile build/src/Makefile.orig
	sed -e '74s#INSTALL_ROOT\s*=#INSTALL_ROOT=/usr#' build/src/Makefile.orig > build/src/Makefile
	rm build/src/Makefile.orig
	cp build/src/Makefile build/src/Makefile.orig
	sed -e '83s#/inc#/include#' build/src/Makefile.orig > build/src/Makefile
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} make && ${SB2} make install)
	touch $@

clobber::
	rm -rf build
