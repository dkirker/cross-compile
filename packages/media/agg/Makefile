NAME    = agg
VERSION = 2.5
DEPENDS = 

SRC_TGZ = http://www.antigrain.com/agg-2.5.tar.gz

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} $(MAKE) )
	 # Automatic staging doesn't work for agg
	mkdir -p ${STAGING_DIR}/usr/lib/
	cp -pP build/$*/src/libagg.a ${STAGING_DIR}/usr/lib/
	mkdir -p ${STAGING_DIR}/usr/include/agg2
	cp -prP build/$*/include/* ${STAGING_DIR}/usr/include/agg2

	touch $@

clobber::
	rm -rf build
