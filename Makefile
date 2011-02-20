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

#
# Primary targets (all architectures at once)
#

# Build everything (so long you probably do not want to do it!)
.PHONY: all
all: stage

# Prepare everything for build but stop just before going for it
# (so one can later build a package & rely on dependencies for just what is needed)
.PHONY: setup
setup: toolchain rootfs map depend

#
# Alternate primary targets (only for step-by-step build on all architectures)
#

ARCH_LIST = armv7 armv6 i686

TOOLCHAIN_ARCH_LIST = $(foreach arch,$(ARCH_LIST),toolchain-$(arch))
ROOTFS_ARCH_LIST    = $(foreach arch,$(ARCH_LIST),   rootfs-$(arch))
MAP_ARCH_LIST       = $(foreach arch,$(ARCH_LIST),      map-$(arch))
SETUP_ARCH_LIST     = $(foreach arch,$(ARCH_LIST),    setup-$(arch))
STAGE_ARCH_LIST     = $(foreach arch,$(ARCH_LIST),    stage-$(arch))

.PHONY: toolchain
toolchain: $(TOOLCHAIN_ARCH_LIST)

.PHONY: rootfs
rootfs: $(ROOTFS_ARCH_LIST)

.PHONY: map
map: $(MAP_ARCH_LIST)

.PHONY: stage
stage: $(STAGE_ARCH_LIST)


#
# Secondary targets (architecture-by-architecture)
#

.PHONY: $(STAGE_ARCH_LIST)
stage-% : toolchain-% rootfs-% map-% depend
	$(MAKE) -C . ARCH=$* INC_DEPS=1 buildall

.PHONY: $(SETUP_ARCH_LIST)
setup-% : toolchain-% rootfs-% map-% depend

#
# Alternate secondary targets (only for step-by-step build on one architectures)
#

.PHONY: $(TOOLCHAIN_ARCH_LIST)
toolchain-armv7: toolchain-arm-1.x \
	   	 toolchain-arm-2.x
toolchain-armv6: toolchain-arm-1.x
toolchain-i686:  toolchain-emu-1.x

.PHONY: toolchain-arm-1.x toolchain-arm-2.x toolchain-emu-1.x
toolchain-arm-1.x: toolchain/arm-2007q3/.unpacked
toolchain-arm-2.x: toolchain/arm-2009q1/.unpacked
toolchain-emu-1.x: toolchain/i686-unknown-linux-gnu/.unpacked \
	           doctors/Palm_webOS_SDK-Mac-1.4.5.465.pkg

.PHONY: $(ROOTFS_ARCH_LIST)
rootfs-% : rootfs/%/.unpacked

.PHONY: $(MAP_ARCH_LIST)
map-% : staging/mapping-%

include support/build.mk

.PHONY: depend
depend: $(dep_files)

#
# Targets & rules to satisfy secondary targets
#

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

.PHONY: clean
clean:
	rm -f .*~ *~ scripts/*~ support/*~ packages/*/*~

.PHONY: clobber
clobber: clobber-armv7 clobber-armv6 clobber-i686
	rm -rf toolchain rootfs staging

.PHONY: clobber-%
clobber-%:
	$(MAKE) -C . ARCH=$* INC_DEPS=1 clobberall
