NAME = scummvm
VERSION = 1.0.0

SRC_BZ2 = http://sourceforge.net/projects/scummvm/files/scummvm/1.0.0/scummvm-1.0.0.tar.bz2/download

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.bz2
	rm -rf build
	mkdir -p build
	tar -C build -j -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; sb2 -t $* ./configure \
		--backend=sdl --with-sdl-prefix=`cd ../../${STAGING_DIR}; pwd`/usr/bin )
	( cd build/src ; sb2 -t $* make)
	touch $@

clobber::
	rm -rf build
