# Packages up xterm...
NAME = xterm-package
VERSION = 0.4.0
DEPENDS = \
	x/xterm

SRC_GIT = git://git.webos-internals.org/x11/xterm-launcher.git
SRC_GIT_COMMIT = git@git.webos-internals.org:x11/xterm-launcher.git

include ../../../support/download.mk
include ../../../support/staging.mk
include ../../../support/cross-compile.mk

stage-local:: build


.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	rm -f $@
	mkdir -p build/src
	tar -C build/src -x -v -f $<
	touch $@

build: build/${ARCH}.built

src-update: build/.unpacked
	( cd build/src && git checkout master && git pull )

build/%.built: src-update
	rm -f $@
	( rm -rf build/$* )
	( mkdir -p build/$* )
	( cp build/src/${ARCH}/xecutah.sh build/$* )
	( cp build/src/${ARCH}/icon.png build/$* )
	( cp build/src/${ARCH}/appinfo.json build/$* )
	( cp build/src/${ARCH}/package.properties build/$* )
	( mkdir -p build/$*/bin )
	( cp ../xterm/build/$*/xterm build/$*/bin/ )
	( ${SB2} strip build/$*/bin/* )
	touch $@

# TODO: Add armv6 support
package-commit: clobber stage
	if [ -d git ]; then \
		(cd git && git checkout . && git pull); \
	else \
		git clone "${SRC_GIT_COMMIT}" git; \
	fi
	cd git && rm -rf *
	mkdir -p git/armv7
	cp -r build/armv7/* git/armv7/
	mkdir -p git/armv6
	-cp -r build/armv6/* git/armv6/
	( cd git && git add . && git commit -am "(Automatic commit)" )

package:
	rm -rf build/ipk
	mkdir -p build/ipk
	palm-package -o build/ipk build/armv7
	#palm-package build/armv6

install:
	-palm-install -r org.webosinternals.xterm
	palm-install build/ipk/*.ipk

test: stage package install

clobber::
	rm -rf build

