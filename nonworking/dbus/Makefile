NAME = dbus
VERSION = 1.3.0

SRC_TGZ = http://dbus.freedesktop.org/releases/dbus/${NAME}-${VERSION}.tar.gz

include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	touch $@

include ../../support/staging.mk

stage:: build

include ../../support/cross-compile.mk

build: build/armv7.built

build/%.built: build/.unpacked
	rm -f $@
	( cd build/src ; ${SB2} \
	PKG_CONFIG_LIBDIR=/usr/local/lib/pkgconfig:/usr/local/share/pkgconfig \
	LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" \
	./configure \
		--prefix=${INSTALL_PREFIX} \
	)	
	#XXX Not sure why this is a problem, refer:
	#XXX http://lists.freedesktop.org/archives/dbus/2009-October/011825.html
	( cd build/src ; sed --in-place -e 's/#ifndef F_DUPFD_CLOEXEC/#ifdef F_DUPFD_CLOEXEC/g' dbus/dbus-sysdeps-unix.c)
	( cd build/src ; ${SB2} make)
	( cd build/src ; ${SB2} make install)
	touch $@

clobber::
	rm -rf build
