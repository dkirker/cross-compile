#
# due to copyright reasons you need to create the file
# pd1-1.4.tar.gz yourself, here is how:
#
#  - download the dmg file
#  - mount it
#  - copy PalmPDK.pkg to your desktop
#  - open terminal and cd to Desktop
#  - rename it: mv PalmPDK.pkg pdl-1.4
#  - tar it: tar cpzf pdl-1.4.tar.gz pdl-1.4
#  - put this file to the crosscompile downloads dir
#    (from here: ../../downloads)
#
NAME    = pdl
VERSION = 1.4
DEPENDS = 
PAXFILE = Contents/Packages/palmpdk.pkg/Contents/Archive.pax.gz
HEADERS = build/src/opt/PalmPDK/include/PDL*
LIBS    = build/src/opt/PalmPDK/device/lib/libpdl.so
PDLIPK  = build/src/opt/PalmPDK/share/packages/Native-SDK/castle/libpdl_1.0-32_armv7.ipk \
	  build/src/opt/PalmPDK/share/packages/Native-SDK/pixie/libpdl_1.0-32_armv6.ipk


include ../../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build/src
	tar -C build -z -x -f ${DL_DIR}/${NAME}-${VERSION}.tar.gz
	mv build/${NAME}-${VERSION} build/pkg
	(cd build/src; gunzip -c ../pkg/${PAXFILE} | cpio -i )
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage:: build/armv7.built


build/%.built: build/.unpacked
	( ${SB2} cp ${HEADERS} /usr/local/include/; ${SB2} cp ${LIBS} /usr/local/lib/ )
	cp ${PDLIPK} .
	@echo "*** PDL installed. You need to install the libpdl ipkg on the device ***"
	touch $@





clobber::
	rm -rf build
