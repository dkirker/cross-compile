#
# python + python-dev required for building!
#

NAME = gpsd
VERSION = 2.90
DEPENDS =

SRC_TGZ = http://download.berlios.de/gpsd/gpsd-2.90.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/gpsd-${VERSION} build/src
	touch $@

include ../../support/staging.mk

include ../../support/stage-rootfs.mk

include ../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} export CFLAGS=-I${INSTALL_PREFIX}/include && \
		export LDFLAGS="-L${INSTALL_PREFIX}/lib -Wl,-rpath,${INSTALL_PREFIX}/lib" && \
		./configure --disable-static --prefix=${INSTALL_PREFIX} \
		--without-x )
	( cd build/src ; /usr/bin/python2.5 jsongen.py --ais --target=parser >ais_json.i )
	( cd build/src ; /usr/bin/python2.5 maskaudit.py -c >maskdump.c )
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
