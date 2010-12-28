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
toolchain: toolchain/arm-2009q1/.unpacked toolchain/arm-2007q3/.unpacked \
	   toolchain/i686-unknown-linux-gnu/.unpacked doctors/Palm_webOS_SDK-Mac-1.4.5.465.pkg

.PHONY: rootfs
# rootfs: rootfs/armv7/.unpacked rootfs/armv6/.unpacked rootfs/i686/.unpacked
rootfs: rootfs/armv7/.unpacked rootfs/armv6/.unpacked

.PHONY: stage
# stage: toolchain rootfs staging-armv7 staging-armv6 staging-i686
stage: toolchain rootfs
	$(MAKE) -C . staging-armv6
	$(MAKE) -C . staging-armv7

include support/build.mk

.PHONY: staging-%
staging-%: toolchain rootfs staging/mapping-% $(dep_files)
	$(MAKE) -C . ARCH=$* INC_DEPS=1 buildall

.PRECIOUS: staging/mapping-%
staging/mapping-%:
	mkdir -p staging/$*/usr
	sed -e "/99. Other rules./a\
		{prefix = \"/usr/local\", replace_by = \"`pwd`/staging/$*/usr\"}, \
		\n{prefix = \"/usr/lib64/perl\", map_to = tools}, \
	" \
		$(SB2ROOT)/share/scratchbox2/lua_scripts/pathmaps/simple/00_default.lua > $@

rootfs/armv7/.unpacked: doctors/webosdoctorp100ueu-wr-1.4.5.jar
	rm -rf rootfs/armv7
	mkdir -p rootfs/armv7
	${FAKEROOT} scripts/unpack-doctor-rootfs $< rootfs/armv7
	touch $@

rootfs/armv6/.unpacked: doctors/webosdoctorp121ewweu-wr-1.4.5.jar
	rm -rf rootfs/armv6
	mkdir -p rootfs/armv6
	${FAKEROOT} scripts/unpack-doctor-rootfs $< rootfs/armv6
	touch $@

rootfs/i686/.unpacked: doctors/palm-sdk_1.4.5-svn307799-sdk1457-pho465_i386.deb
	rm -rf rootfs/i686
	mkdir -p rootfs/i686
#	%%% Need to do something here %%%
	false
	touch $@

toolchain/arm-2009q1/.unpacked: downloads/arm-2009q1-203-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

toolchain/arm-2007q3/.unpacked: downloads/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

toolchain/i686-unknown-linux-gnu/.unpacked: downloads/i686-unknown-linux-gnu-1.4.1.tar.gz
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

doctors/Palm_webOS_SDK-Mac-1.4.5.465.pkg: doctors/Palm_webOS_SDK.1.4.5.465.dmg
	${MAKE} -C packages/host/dmg2img stage-local
	mkdir -p toolchain/sdk/mnt
	packages/host/dmg2img/build/src/dmg2img -p 4 \
		-i doctors/Palm_webOS_SDK.1.4.5.465.dmg \
		-o toolchain/sdk/Palm_webOS_SDK.1.4.5.465.hfs
	sudo mount -t hfsplus -o loop \
		toolchain/sdk/Palm_webOS_SDK.1.4.5.465.hfs \
		toolchain/sdk/mnt
	cp toolchain/sdk/mnt/Palm_webOS_SDK-Mac-1.4.5.465.pkg $@
	sudo umount toolchain/sdk/mnt
	rm -rf toolchain/sdk

downloads/arm-2009q1-203-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2:
	mkdir -p downloads
	wget -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package4571/public/arm-none-linux-gnueabi/arm-2009q1-203-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

downloads/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2:
	mkdir -p downloads
	wget -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package1787/public/arm-none-linux-gnueabi/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

downloads/i686-unknown-linux-gnu-1.4.1.tar.gz:
	mkdir -p downloads
	wget -O $@ http://sources.nslu2-linux.org/sources/i686-unknown-linux-gnu-1.4.1.tar.gz

doctors/webosdoctorp100ueu-wr-1.4.5.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pre/p145r0d06302010/eudep145rod/webosdoctorp100ueu-wr.jar

doctors/webosdoctorp121ewweu-wr-1.4.5.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pixiplus/px145r0d06302010/wrep145rod/webosdoctorp121ewweu-wr.jar

doctors/palm-sdk_1.4.5-svn307799-sdk1457-pho465_i386.deb:
	mkdir -p doctors
	wget -O $@ http://cdn.downloads.palm.com/sdkdownloads/1.4.5.465/sdkBinaries/palm-sdk_1.4.5-svn307799-sdk1457-pho465_i386.deb

doctors/Palm_webOS_SDK.1.4.5.465.dmg:
	mkdir -p doctors
	wget -O $@ http://cdn.downloads.palm.com/sdkdownloads/1.4.5.465/sdkBinaries/Palm_webOS_SDK.1.4.5.465.dmg

.PHONY: clean
clean:
	rm -f .*~ *~ scripts/*~ support/*~ packages/*/*~

.PHONY: clobber
clobber: clobber-armv7 clobber-armv6 clobber-i686
	rm -rf toolchain rootfs staging

.PHONY: clobber-%
clobber-%:
	$(MAKE) -C . ARCH=$* INC_DEPS=1 clobberall
