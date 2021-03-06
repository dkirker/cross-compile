SB2ROOT = $(shell dirname `which sb2`)/..

ifeq ($(shell uname -s),Darwin)
TAR	 = gnutar
MD5SUM	 = md5
else
TAR	 = tar
MD5SUM	 = md5sum
endif

.PHONY: all
all: stage

ARCH_LIST = armv7 armv6 i686

TOOLCHAIN_ARCH_LIST = $(foreach arch,$(ARCH_LIST),toolchain-$(arch))
ROOTFS_ARCH_LIST    = $(foreach arch,$(ARCH_LIST),   rootfs-$(arch))
SETUP_ARCH_LIST     = $(foreach arch,$(ARCH_LIST),    setup-$(arch))
DOWNLOAD_ARCH_LIST  = $(foreach arch,$(ARCH_LIST), download-$(arch))
STAGE_ARCH_LIST     = $(foreach arch,$(ARCH_LIST),    stage-$(arch))

.PHONY: toolchain
toolchain: $(TOOLCHAIN_ARCH_LIST)

.PHONY: rootfs
rootfs: ${ROOTFS_ARCH_LIST}

.PHONY: setup
setup: ${SETUP_ARCH_LIST}

.PHONY: download
download: ${DOWNLOAD_ARCH_LIST}

.PHONY: stage
stage: ${STAGE_ARCH_LIST}

include support/build.mk

.PHONY: $(TOOLCHAIN_ARCH_LIST)
toolchain-armv7: toolchain-arm-1.x \
	   	 toolchain-arm-2.x \
	         doctors/Palm_webOS_SDK-Mac-1.4.5.465.pkg \
		 doctors/Palm_webOS_SDK-Mac-3.0.0.643.mpkg
toolchain-armv6: toolchain-arm-1.x \
	         doctors/Palm_webOS_SDK-Mac-1.4.5.465.pkg \
		 doctors/Palm_webOS_SDK-Mac-3.0.0.643.mpkg
toolchain-i686:  toolchain-emu-1.x \
	         doctors/Palm_webOS_SDK-Mac-1.4.5.465.pkg \
		 doctors/Palm_webOS_SDK-Mac-3.0.0.643.mpkg

.PHONY: toolchain-arm-1.x toolchain-arm-2.x toolchain-emu-1.x
toolchain-arm-1.x: toolchain/arm-2007q3/.unpacked
toolchain-arm-2.x: toolchain/arm-2009q1/.unpacked
toolchain-emu-1.x: toolchain/i686-unknown-linux-gnu/.unpacked

.PHONY: rootfs-%
rootfs-%: rootfs/%/.unpacked
	@true

.PHONY: setup-%
setup-%: toolchain-% rootfs-% staging/mapping-% $(dep_files)
	@true

.PHONY: download-%
download-%: toolchain-% rootfs-% staging/mapping-% $(dep_files)
	$(MAKE) -C . ARCH=$* INC_DEPS=1 downloadall

.PHONY: stage-%
stage-%: toolchain-% rootfs-% staging/mapping-% $(dep_files)
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
	scripts/unpack-doctor-rootfs $< rootfs/armv7
	touch $@

rootfs/armv6/.unpacked: doctors/webosdoctorp121ewweu-wr-1.4.5.jar
	rm -rf rootfs/armv6
	mkdir -p rootfs/armv6
	scripts/unpack-doctor-rootfs $< rootfs/armv6
	touch $@

rootfs/i686/.unpacked: doctors/palm-sdk_1.4.5-svn307799-sdk1457-pho465_i386.deb
	rm -rf rootfs/i686
	mkdir -p rootfs/i686/extract/mnt
	ar p $< data.tar.gz | tar -C rootfs/i686/extract -x -z
	7z x -y -orootfs/i686/extract "rootfs/i686/extract/opt/PalmSDK/Current/share/emulator/images/SDK 1.4.5.465.vmdk.zip"
	VBoxManage clonehd rootfs/i686/extract/nova-cust-image-sdk1457.vmdk rootfs/i686/extract/nova-cust-image-sdk1457.raw --format RAW
	rm -f rootfs/i686/extract/nova-cust-image-sdk1457.vmdk
	7z x -y -orootfs/i686/extract rootfs/i686/extract/nova-cust-image-sdk1457.raw 0.img
	rm -f rootfs/i686/extract/nova-cust-image-sdk1457.raw
	sudo mount -oloop rootfs/i686/extract/0.img rootfs/i686/extract/mnt
	sudo tar -C rootfs/i686/extract/mnt -c . --exclude=./dev | tar -C rootfs/i686 -x || true	# Get rid of rights
	sudo umount rootfs/i686/extract/mnt
	rm -rf rootfs/i686/extract
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
	rm -rf $@
	mkdir -p toolchain/sdk/mnt
	7z x -y -so $< 4.hfs > toolchain/sdk/Palm_webOS_SDK.1.4.5.465.hfs
	7z x -y -otoolchain/sdk toolchain/sdk/Palm_webOS_SDK.1.4.5.465.hfs Palm_webOS_SDK/Palm_webOS_SDK-Mac-1.4.5.465.pkg
	mv toolchain/sdk/Palm_webOS_SDK/Palm_webOS_SDK-Mac-1.4.5.465.pkg $@
	rm -rf toolchain/sdk

doctors/Palm_webOS_SDK-Mac-2.1.0.519.mpkg: doctors/Palm_webOS_SDK.2.1.0.519.dmg
	rm -rf $@
	mkdir -p toolchain/sdk/mnt
	7z x -y -so $< 4.hfs > toolchain/sdk/Palm_webOS_SDK.2.1.0.519.hfs
	7z x -y -otoolchain/sdk toolchain/sdk/Palm_webOS_SDK.2.1.0.519.hfs Palm_webOS_SDK/Palm_webOS_SDK-Mac-2.1.0.519.mpkg
	mv toolchain/sdk/Palm_webOS_SDK/Palm_webOS_SDK-Mac-2.1.0.519.mpkg $@
	rm -rf toolchain/sdk

doctors/Palm_webOS_SDK-Mac-3.0.0.643.mpkg: doctors/Palm_webOS_SDK.3.0.0.643.dmg
	rm -rf $@
	mkdir -p toolchain/sdk/mnt
	7z x -y -so $< 4.hfs > toolchain/sdk/Palm_webOS_SDK.3.0.0.643.hfs
	7z x -y -otoolchain/sdk toolchain/sdk/Palm_webOS_SDK.3.0.0.643.hfs Palm_webOS_SDK/Palm_webOS_SDK-Mac-3.0.0.643.mpkg
	mv toolchain/sdk/Palm_webOS_SDK/Palm_webOS_SDK-Mac-3.0.0.643.mpkg $@
	rm -rf toolchain/sdk

downloads/arm-2009q1-203-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2:
	mkdir -p downloads
	wget -c -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package4571/public/arm-none-linux-gnueabi/arm-2009q1-203-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

downloads/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2:
	mkdir -p downloads
	wget -c -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package1787/public/arm-none-linux-gnueabi/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

downloads/i686-unknown-linux-gnu-1.4.1.tar.gz:
	mkdir -p downloads
	wget -c -O $@ http://sources.nslu2-linux.org/sources/i686-unknown-linux-gnu-1.4.1.tar.gz

doctors/webosdoctorp100ueu-wr-1.4.5.jar:
	mkdir -p doctors
	wget -c -O $@ http://palm.cdnetworks.net/rom/pre/p145r0d06302010/eudep145rod/webosdoctorp100ueu-wr.jar

doctors/webosdoctorp121ewweu-wr-1.4.5.jar:
	mkdir -p doctors
	wget -c -O $@ http://palm.cdnetworks.net/rom/pixiplus/px145r0d06302010/wrep145rod/webosdoctorp121ewweu-wr.jar

doctors/palm-sdk_1.4.5-svn307799-sdk1457-pho465_i386.deb:
	mkdir -p doctors
	wget -c -O $@ http://cdn.downloads.palm.com/sdkdownloads/1.4.5.465/sdkBinaries/palm-sdk_1.4.5-svn307799-sdk1457-pho465_i386.deb

doctors/Palm_webOS_SDK.1.4.5.465.dmg:
	mkdir -p doctors
	wget -c -O $@ http://cdn.downloads.palm.com/sdkdownloads/1.4.5.465/sdkBinaries/Palm_webOS_SDK.1.4.5.465.dmg

doctors/Palm_webOS_SDK.2.1.0.519.dmg:
	mkdir -p doctors
	wget -c -O $@ http://cdn.downloads.palm.com/sdkdownloads/2.1.0.519/sdkBinaries/Palm_webOS_SDK.2.1.0.519.dmg

doctors/Palm_webOS_SDK.3.0.0.643.dmg:
	mkdir -p doctors
	wget -c -O $@ https://cdn.downloads.palm.com/sdkdownloads/3.0.0.643/sdkBinaries/Palm_webOS_SDK.3.0.0.643.dmg

doctors/webosdoctorp100ueu-wr-2.1.0.jar:
	mkdir -p doctors
	wget -c -O $@ http://palm.cdnetworks.net/rom/preplus/p210r0d03142011/eudep210rod/webosdoctorp101ueu-wr.jar

doctors/palm-sdk_2.1.0-svn409992-pho519_i386.deb:
	mkdir -p doctors
	wget -c -O $@ https://cdn.downloads.palm.com/sdkdownloads/2.1.0.519/sdkBinaries/palm-sdk_2.1.0-svn409992-pho519_i386.deb

.PHONY: clean
clean:
	rm -f .*~ *~ scripts/*~ support/*~ packages/*/*~

CLOBBER_ARCH_LIST = $(foreach arch,$(ARCH_LIST), clobber-$(arch))

.PHONY: clobber
clobber: ${CLOBBER_ARCH_LIST}
	rm -rf toolchain rootfs staging

.PHONY: clobber-%
clobber-%:
	$(MAKE) -C . ARCH=$* INC_DEPS=1 clobberall
