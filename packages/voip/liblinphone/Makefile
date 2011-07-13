NAME = linphone
VERSION = 3.3.2
DEPENDS = voip/libosip2 voip/libeXosip2 media/gsm media/libspeex media/pulseaudio common/ncurses common/readline

SRC_TGZ = http://download.savannah.gnu.org/releases/linphone/3.3.x/sources/linphone-3.3.2.tar.gz
PATCH   = linphone-3.3.2-fix.patch

CFLAGS_OPT= -O2

include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

.SECONDARY: build/.unpacked
build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -rf build
	mkdir -p build
	tar -C build -z -x -f $<
	mv build/${NAME}-${VERSION} build/src
	[ -n "${PATCH}" ] && patch -d build/src -p 1 < ${PATCH}
	(cd build/src; etags `find . -type f -print`)
	touch $@

include ../../../support/staging.mk

#include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

configure:: build/${ARCH}.configured
stage-local:: build/${ARCH}.built
#install:: build/${ARCH}.installed

.SECONDARY: build/${ARCH}.aclocal
build/%.aclocal: build/.unpacked
	rm -f $@
	rm -rf build/$*
	mkdir -p build/$*
# cp is notorious for compromising last-modification time...
# so rely on tar instead until autoconf/automake weirdness is fixed!
#	cp -R build/src/* build/$*
	(cd build/src; tar cf - .) | (cd build/$*; tar xf -)
#	( cd build/$* ; ${SB2} ./autogen.sh )
	touch $@

.SECONDARY: build/${ARCH}.configured
build/%.configured: build/${ARCH}.aclocal
	rm -f $@
	( cd build/$* ; ${SB2} CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ ${CFLAGS_OPT}" \
	        LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
	        PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
	        ./configure --prefix=${INSTALL_PREFIX} --enable-gtk_ui=no --disable-video --disable-strict --disable-nls --enable-alsa=no --enable-oss=no --enable-pulseaudio=yes --with-gsm=${INSTALL_PREFIX} )
	touch $@


build/%.built: build/${ARCH}.configured
	rm -f $@
	( cd build/$* ; ${SB2} $(MAKE) CFLAGS="-I${INSTALL_PREFIX}/include\ -g\ ${CFLAGS_OPT}" \
	        LDFLAGS="-L${INSTALL_PREFIX}/lib\ -Wl,-rpath,${INSTALL_PREFIX}/lib" \
	        PKG_CONFIG_PATH=${INSTALL_PREFIX}/lib/pkgconfig \
	        install prefix=${INSTALL_PREFIX} )
	touch $@

clobber::
	rm -rf build
