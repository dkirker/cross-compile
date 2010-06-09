DOCTORPRE  = webosdoctorp100ewwsprint-1.3.5.jar
DOCTORPIXI = webosdoctorp200ewwsprint-1.3.5.jar
SB2ROOT = $(shell dirname `which sb2`)/..

ifeq ($(shell uname -s),Darwin)
TAR	 = gnutar
MD5SUM	 = md5
FAKEROOT = sudo
else
TAR	 = tar
MD5SUM	 = md5sum
FAKEROOT = fakeroot
endif

.PHONY: all
all: toolchain rootfs stage

.PHONY: setup
setup: toolchain rootfs staging/mapping-armv7 staging/mapping-armv6 staging/mapping-i686

.PHONY: toolchain
toolchain: toolchain/arm-2007q3/.unpacked toolchain/i686-unknown-linux-gnu/.unpacked

.PHONY: rootfs
# rootfs: rootfs/armv7/.unpacked rootfs/armv6/.unpacked rootfs/i686/.unpacked
rootfs: rootfs/armv7/.unpacked

.PHONY: stage
# stage: toolchain rootfs staging-armv7 staging-armv6 staging-i686
stage: toolchain rootfs staging-armv7

include support/build.mk

.PHONY: staging-%
staging-%: staging/mapping-% $(dep_files)
	$(MAKE) -C . ARCH=$* INC_DEPS=1 buildall

.PRECIOUS: staging/mapping-%
staging/mapping-%:
	mkdir -p staging/$*/usr
	sed -e "/99. Other rules./a\
		{prefix = \"/usr/local\", replace_by = \"`pwd`/staging/$*/usr\"}, \
		\n{prefix = \"/usr/lib64/perl\", map_to = tools}, \
	" \
		$(SB2ROOT)/share/scratchbox2/lua_scripts/pathmaps/simple/00_default.lua > $@

rootfs/armv7/.unpacked: doctors/webosdoctorp100ewwsprint-1.4.1.1.jar
	rm -rf rootfs/armv7
	mkdir -p rootfs/armv7
	${FAKEROOT} scripts/unpack-doctor-rootfs $< rootfs/armv7
	touch $@

rootfs/armv6/.unpacked: doctors/webosdoctorp200ewwsprint-1.4.1.1.jar
	rm -rf rootfs/armv6
	mkdir -p rootfs/armv6
	${FAKEROOT} scripts/unpack-doctor-rootfs $< rootfs/armv6
	touch $@

rootfs/i686/.unpacked: doctors/palm-sdk_1.3.5-svn234138-sdk117-pho368_i386.deb
	rm -rf rootfs/i686
	mkdir -p rootfs/i686
	touch $@

toolchain/arm-2007q3/.unpacked: downloads/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

toolchain/i686-unknown-linux-gnu/.unpacked: downloads/i686-unknown-linux-gnu-1.4.1.tar.gz
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

downloads/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2:
	mkdir -p downloads
	wget -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package1787/public/arm-none-linux-gnueabi/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

downloads/i686-unknown-linux-gnu-1.4.1.tar.gz:
	mkdir -p downloads
	wget -O $@ http://sources.nslu2-linux.org/sources/i686-unknown-linux-gnu-1.4.1.tar.gz

doctors/webosdoctorp100ewwsprint-1.4.1.1.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pre/p1411r0d03312010/sr1ntp1411rod/webosdoctorp100ewwsprint.jar

doctors/webosdoctorp200ewwsprint-1.4.1.1.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pixi/px1411r0d03312010/sr1ntp1411rod/webosdoctorp200ewwsprint.jar

doctors/palm-sdk_1.3.5-svn234138-sdk117-pho368_i386.deb:
	mkdir -p doctors
	wget -O $@ --no-check-certificate https://cdn.downloads.palm.com/sdkdownloads/1.3.5.368/sdkBinaries/palm-sdk_1.3.5-svn234138-sdk117-pho368_i386.deb

doctors/PalmPDK.pkg.tar.gz:
	mkdir -p doctors
	echo "Download the MacOSX PDK, mount it, run tar zcvf PalmPDK.pkg.tar.gz PalmPDK.pkg and copy the resulting file into doctors"

.PHONY: clean
clean:
	rm -f .*~ *~ scripts/*~ support/*~ packages/*/*~

.PHONY: clobber
clobber: clobber-armv7 clobber-armv6 clobber-i686
	rm -rf toolchain rootfs staging

.PHONY: clobber-%
clobber-%:
	$(MAKE) -C . ARCH=$* INC_DEPS=1 clobberall
