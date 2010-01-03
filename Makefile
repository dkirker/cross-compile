TOOLCHAIN  = arm-2007q3
DOCTORPRE  = webosdoctorp100ewwsprint-1.3.5.jar
DOCTORPIXI = webosdoctorp200ewwsprint-1.3.5.jar

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
all: toolchain rootfs

.PHONY: toolchain
toolchain: toolchain/${TOOLCHAIN}/.unpacked

.PHONY: rootfs
# rootfs: rootfs/armv7/.unpacked rootfs/armv6/.unpacked
rootfs: rootfs/armv7/.unpacked

rootfs/armv7/.unpacked: doctors/webosdoctorp100ewwsprint-1.3.5.jar
	rm -rf rootfs/armv7
	mkdir -p rootfs/armv7
	${FAKEROOT} scripts/unpack-doctor-rootfs $< rootfs/armv7
	touch $@

rootfs/armv6/.unpacked: doctors/webosdoctorp200ewwsprint-1.3.5.jar
	rm -rf rootfs/armv6
	mkdir -p rootfs/armv6
	${FAKEROOT} scripts/unpack-doctor-rootfs $< rootfs/armv6
	touch $@

toolchain/arm-2007q3/.unpacked: downloads/arm-2007q3-53-arm-none-eabi-i686-pc-linux-gnu.tar.bz2
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

downloads/arm-2007q3-53-arm-none-eabi-i686-pc-linux-gnu.tar.bz2:
	mkdir -p downloads
	wget -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package1793/public/arm-none-eabi/arm-2007q3-53-arm-none-eabi-i686-pc-linux-gnu.tar.bz2

doctors/webosdoctorp100ewwsprint-1.3.5.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pre/p135r0d12302009/sr1ntp135rod/webosdoctorp100ewwsprint.jar

doctors/webosdoctorp200ewwsprint-1.3.5.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pixi/px135r0d12302009/sr1ntp135rod/webosdoctorp200ewwsprint.jar

clean:
	rm -f .*~ *~ scripts/*~

clobber:
	rm -rf toolchain rootfs
