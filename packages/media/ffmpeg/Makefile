NAME = ffmpeg
VERSION = 16648
DEPENDS = 

SRC_BZ2 = http://palm.cdnetworks.net/opensource/1.4.1.1/ffmpeg_${VERSION}.tar.gz
PATCH = http://palm.cdnetworks.net/opensource/1.4.1.1/ffmpeg_${VERSION}_patches.tgz

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: 
	rm -rf build
	mkdir -p build
	curl ${SRC_BZ2} | tar -C build -zxf -
	mv build/trunk build/src
	curl ${PATCH} | tar xvOzf - | patch -d build/src -p1
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2}	./configure --prefix=${INSTALL_PREFIX} --enable-swscale --enable-gpl )
	( cd build/src; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
