NAME    = libpdl
VERSION = 3.0.0.643
DEPENDS = common/luna-prefs common/libcurl
HEADERS = build/opt/PalmPDK/include/PDL*
LIBRARIES = build/opt/PalmPDK/device/lib/libpdl.so

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ../../../doctors/Palm_webOS_SDK-Mac-${VERSION}.mpkg
	rm -rf build
	mkdir -p build
	( cd build ; gunzip -c ../$</Contents/Packages/palmpdk.pkg/Contents/Archive.pax.gz | cpio -i )
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: build/.unpacked
	${SB2} cp ${HEADERS} /usr/local/include/
	${SB2} cp ${LIBRARIES} /usr/local/lib/
	touch $@

clobber::
	rm -rf build

download::
