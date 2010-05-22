NAME = icewm
VERSION = 1.2.37

SRC_TGZ = http://downloads.sourceforge.net/project/icewm/icewm-stable/${VERSION}/${NAME}-${VERSION}.tar.gz?use_mirror=cdnetworks-us-2

DEPENDS = ../nonworking/libXpm \
	  ../nonworking/libXrandr \
	  ../nonworking/fontconfig \
	  ../nonworking/libXft \
	  ../nonworking/libXi \
	  ../nonworking/libXrender \
	  ../nonworking/xinput \
	  ../nonworking/libXext \
	  ../nonworking/libx11 \
	  ../nonworking/xproto

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

stage:: build

include ../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
	 ./configure \
		--prefix=${INSTALL_PREFIX} \
		--disable-xinerama \
		--with-xpm=${INSTALL_PREFIX} \
	)	
	#XXX Replace O2 with O0 to work around compiler bug
	( cd build/src ; sed --in-place -e 's/-g -O2/-g1 -O0/g' src/Makefile)
	( cd build/src ; ${SB2} make)
	( cd build/src ; ${SB2} make install)
	touch $@

clobber::
	rm -rf build
