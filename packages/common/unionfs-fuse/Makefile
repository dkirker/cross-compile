NAME = unionfs-fuse
VERSION = 0.23
DEPENDS = common/fuse

SRC_BZ2 = http://podgorny.cz/unionfs-fuse/releases/unionfs-fuse-0.23.tar.bz2

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: 
	rm -rf build
	mkdir -p build
	curl ${SRC_BZ2} | tar -C build -jxf -
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$*/src ; ${SB2}	CC=gcc LDFLAGS="\"-L/usr/local/lib -Wl,-rpath,/usr/local/lib\"" $(MAKE) )
	( ${SB2} install -d /usr/local/bin )
	( ${SB2} install -m 644 build/$*/src/unionfs /usr/local/bin/unionfs )
	touch $@

clobber::
	rm -rf build

download::
