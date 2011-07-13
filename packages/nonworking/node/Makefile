NAME = node
VERSION = 0.4.8
DEPENDS = python/python

SRC_TGZ = http://nodejs.org/dist/node-v$(VERSION).tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/node-v${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local: build/${ARCH}.built

# 'node' binary ends up in build/$(ARCH)/build/default/node, fwiw
build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} ./configure --prefix=$(INSTALL_PREFIX))
	( cd build/$* ; ${SB2} $(MAKE) )
	#( cd build/$* ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
