# Packages up xserver
NAME = xserver-package
VERSION = 0.6.8
DEPENDS = \
	x/xlib-package \
	x/xorg-server \
	x/xkeyboardconfig \
	x/xkbcomp \
	x/xkbset

SRC_GIT = git://git.webos-internals.org/x11/xlauncher.git
SRC_GIT_COMMIT = git@git.webos-internals.org:x11/xlauncher.git

include ../../../support/download.mk
include ../../../support/staging.mk
include ../../../support/cross-compile.mk

stage-local:: build


.PHONY: unpack
unpack: build/.unpacked-${VERSION}

build/.unpacked-${VERSION}: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -f $@
	mkdir -p build/src
	tar -C build/src -x -v -f $<
	touch $@

build: build/${ARCH}.built-${VERSION}

src-update: build/.unpacked-${VERSION}
	( cd build/src && git checkout master && git pull )

build/%.built-${VERSION}: build/.unpacked-${VERSION}
	rm -f $@
	( rm -rf build/$* )
	( mkdir -p build/$* )
	( cp build/src/xecutah.sh build/$* )
	( cp build/src/set_keymap_later.sh build/$* )
	( cp build/src/icon.png build/$* )
	( cp build/src/appinfo.json build/$* )
	( cp build/src/package.properties build/$* )
	( cp -r ../xlib-package/build/$* build/$*/xlib )
	( mkdir -p build/$*/bin )
	( cp ../xorg-server/build/$*/hw/kdrive/sdl/Xsdl build/$*/bin/ )
	( cp ../xorg-server/build/$*/forker build/$*/bin/ )
	( cp ../xkbcomp/build/$*/xkbcomp build/$*/bin/ )
	( cp ../xkbset/build/$*/xkbset build/$*/bin/ )
	( cd ../xkeyboardconfig/build/$*/ && make install prefix=$(PWD)/build/$*/ exec_prefix=$(PWD)/build/$* )
	( cp ../xkeyboardconfig/build/$*/keymap/palm-pre build/$*/share/X11/xkb/keymap/ )
	( cp ../xkeyboardconfig/build/$*/symbols/palm-pre build/$*/share/X11/xkb/symbols/ )
	( cp ../xkeyboardconfig/build/$*/keycodes/palm-pre build/$*/share/X11/xkb/keycodes/ )
	( cp ../xkeyboardconfig/build/$*/keymap/palm-prex build/$*/share/X11/xkb/keymap/ )
	( cp ../xkeyboardconfig/build/$*/symbols/palm-prex build/$*/share/X11/xkb/symbols/ )
	( cp ../xkeyboardconfig/build/$*/keycodes/palm-prex build/$*/share/X11/xkb/keycodes/ )
	( ${SB2} strip build/$*/bin/* )
	touch $@

# TODO: Add armv6 support
package-commit:
	if [ -d git ]; then \
		(cd git && git checkout . && git pull); \
	else \
		git clone "${SRC_GIT_COMMIT}" git; \
	fi
	cd git && rm -rf armv7 armv6
	mkdir -p git/armv7
	cp -r build/armv7/* git/armv7/
	rm -f git/armv7/* || true # don't delete the directories
	mkdir -p git/armv6
	-cp -r build/armv6/* git/armv6/
	rm -f git/armv6/* || true # don't delete the directories
	( cd git && git add . && git commit -am "(Automatic commit)" )

package:
	mkdir -p build/ipk
	rm -rf build/ipk/*.ipk
	palm-package -o build/ipk build/${ARCH}

install:
	-palm-install -r org.webosinternals.xserver
	palm-install build/ipk/*.ipk

test: stage package install

clobber::
	rm -rf build

