NAME = libventrilo3
VERSION = 1026
DEPENDS = media/gsm media/libspeex

DL_DIR = ../../../downloads
SRC_SVN_REV = http://svn.mangler.org/mangler/trunk

download: ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz

${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz:
	rm -f $@
	rm -rf build
	mkdir -p build/trunk
	( cd build ; svn checkout -r ${VERSION} --depth files ${SRC_SVN_REV} )
	( cd build/trunk ; svn checkout -r ${VERSION} ${SRC_SVN_REV}/libventrilo3 )
	( cd build/trunk ; svn checkout -r ${VERSION} ${SRC_SVN_REV}/m4 )
	mkdir -p ${DL_DIR}
	tar -C build/`basename ${SRC_SVN_REV}` -zcf $@ .

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz
	rm -rf build
	mkdir -p build/src
	tar -C build/src -xf ${DL_DIR}/${NAME}-svn-r${VERSION}.tar.gz
	touch $@

build/.patched:
	patch -p0 build/src/configure.ac < patches/configure.ac.patch
	patch -p0 build/src/Makefile.am < patches/Makefile.am.patch
	autoreconf build/src
	touch $@

include ../../../support/staging.mk

stage-local:: build/${ARCH}.built

include ../../../support/cross-compile.mk

build: stage

build/%.built: build/.unpacked build/.patched
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$* ; ${SB2} env LDFLAGS="-L/usr/local/lib" ./configure --disable-glibtest )
	( cd build/$*/libventrilo3 ; ${SB2} $(MAKE) )
	( cd build/$*/libventrilo3 ; ${SB2} $(MAKE) install )
	touch $@

clean:
	( cd build/${ARCH} ; ${SB2} $(MAKE) clean )


clobber::
	rm -rf build
