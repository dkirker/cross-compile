NAME = libgarmin
REVISION = 320
VERSION = ${REVISION}
SVN_URL = http://libgarmin.svn.sourceforge.net/svnroot/libgarmin/libgarmin/dev

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	rm -rf build
	mkdir -p build
	svn checkout -r ${REVISION} -q --non-interactive ${SVN_URL} build/src
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built
#	( cd ${STAGING_DIR}/usr/lib ; \
#     [ -e libgarmin.so ] || ln -s libgarmin.so.1.0.17 libgarmin.so )

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} export CFLAGS=-I${INSTALL_PREFIX}/include && \
		export LDFLAGS="-L${INSTALL_PREFIX}/lib -Wl,-rpath,${INSTALL_PREFIX}/lib" && \
		${SB2} ./autosh.sh &&\
 	        ${SB2} ./configure --prefix=${INSTALL_PREFIX} )
	( cd build/src ; ${SB2} $(MAKE) install )
	touch $@

clobber::
	rm -rf build
