NAME    = pdl
VERSION = 1.4.5
DEPENDS = 
PAXFILE = Contents/Packages/palmpdk.pkg/Contents/Archive.pax.gz
HEADERS = build/opt/PalmPDK/include/PDL*
LIBS    = build/opt/PalmPDK/device/lib/libpdl.so

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked: ../../../toolchain/sdk-1.4.5/PalmPDK.pkg.tar.gz
	mkdir -p build
	tar -C build -z -x -f $<
	( cd build ; gunzip -c ../PalmPDK.pkg/${PAXFILE} | cpio -i )
	touch $@

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

stage-local:: build/armv7.built

build/%.built: build/.unpacked
	${SB2} cp ${HEADERS} /usr/local/include/
	${SB2} cp ${LIBS} /usr/local/lib/
	touch $@

clobber::
	rm -rf build
