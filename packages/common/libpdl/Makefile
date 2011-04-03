NAME    = libpdl
VERSION = 1.4.5.465
DEPENDS = common/luna-prefs common/libcurl
HEADERS = build/opt/PalmPDK/include/PDL*
LIBRARIES = build/opt/PalmPDK/device/lib/libpdl.so

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ../../../doctors/Palm_webOS_SDK-Mac-${VERSION}.pkg
	mkdir -p build
	( cd build ; xar -x -f ../$< palmpdk.pkg )
	( cd build ; gunzip -c palmpdk.pkg/Payload | cpio -i )
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
