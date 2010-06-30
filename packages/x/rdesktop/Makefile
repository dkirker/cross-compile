NAME = rdesktop
VERSION = 1.6.0

#XXX: err, what DOES it depend on?
DEPENDS = x/pango \
		  x/libx11 \
		  x/libice \
		  x/libsm \
		  x/libXext \
		  x/libXrender \
		  x/libXaw \
		  common/glib-2.0

SRC_TGZ = http://prdownloads.sourceforge.net/${NAME}/${NAME}-${VERSION}.tar.gz?download

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

stage-local:: build

include ../../../support/cross-compile.mk

build: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
		--enable-88-color \
	)	
	( cd build/src ; ${SB2} $(MAKE))
	( cd build/src ; ${SB2} $(MAKE) install)
	touch $@

clobber::
	rm -rf build
