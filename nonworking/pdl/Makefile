#
#
NAME    = pdl
VERSION = 1.4
DEPENDS = 
TGZ_URL = http://www.daemon.de/idisk/palmpre/pdk-${VERSION}.tgz
PAXFILE = Contents/Packages/palmpdk.pkg/Contents/Archive.pax.gz
HEADERS = build/src/opt/PalmPDK/include/PDL*
LIBS    = build/src/opt/PalmPDK/device/lib/libpdl.so
PDLIPK  = build/src/opt/PalmPDK/share/packages/Native-SDK/castle/libpdl_1.0-32_armv7.ipk \
	  build/src/opt/PalmPDK/share/packages/Native-SDK/pixie/libpdl_1.0-32_armv6.ipk


include ../../support/download.mk

.PHONY: unpack
unpack: build/.unpacked

${DL_DIR}/pdk-${VERSION}.tgz:
	curl -f -R -L -o ${DL_DIR}/pdk-${VERSION}.tgz ${TGZ_URL}

build/.unpacked: ${DL_DIR}/pdk-${VERSION}.tgz
	mkdir -p build/src
	tar -C build -z -x -f $<
	mv build/pdk-${VERSION} build/pkg
	(cd build/src; gunzip -c ../pkg/${PAXFILE} | cpio -i )
	touch $@

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built


build/%.built: build/.unpacked
	( ${SB2} cp ${HEADERS} /usr/local/include/; ${SB2} cp ${LIBS} /usr/local/lib/ )
	cp ${PDLIPK} .
	@echo "*** PDL installed. You need to install the libpdl ipkg on the device ***"
	touch $@





clobber::
	rm -rf build
