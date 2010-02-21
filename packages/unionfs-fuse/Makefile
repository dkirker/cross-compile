NAME = unionfs-fuse
VERSION = 0.23
DEPENDS = fuse

SRC_BZ2 = http://podgorny.cz/unionfs-fuse/releases/unionfs-fuse-0.23.tar.bz2

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: 
	rm -rf build
	mkdir -p build
	curl ${SRC_BZ2} | tar -C build -jxf -
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src/src ; ${SB2}	CC=gcc LDFLAGS="\"-L/usr/local/lib -Wl,-rpath,/usr/local/lib\"" make )
	( ${SB2} install -m 644 build/src/src/unionfs /usr/local/bin/unionfs )
	touch $@

clobber::
	rm -rf build
