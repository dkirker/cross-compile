NAME = icewm
VERSION = 1.2.37

SRC_TGZ =http://sourceforge.net/projects/icewm/files/icewm-1.2/${VERSION}/${NAME}-${VERSION}.tar.gz/download

DEPENDS = \
	x/libXpm \
	x/libXrandr \
	x/fontconfig \
	x/libXft \
	x/libXi \
	x/libXrender \
	x/xinput \
	x/libXext \
	x/libx11 \
	x/xproto

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built-${VERSION}

build/%.built-${VERSION}: build/.unpacked-${VERSION}
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} \
	LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
	 ./configure \
		--prefix=${INSTALL_PREFIX} \
		--disable-xinerama \
		--with-xpm=${INSTALL_PREFIX} \
	)	
	#XXX Replace O2 with O0 to work around compiler bug
	( cd build/$* ; sed --in-place -e 's/-g -O2/-g1 -O0/g' src/Makefile)
	( cd build/$* ; ${SB2} $(MAKE))
	( cd build/$* ; ${SB2} $(MAKE) install)
	touch $@

clobber::
	rm -rf build
