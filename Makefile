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
all: toolchain rootfs stage

.PHONY: toolchain
toolchain: toolchain/${TOOLCHAIN}/.unpacked

.PHONY: rootfs
# rootfs: rootfs/armv7/.unpacked rootfs/armv6/.unpacked
rootfs: rootfs/armv7/.unpacked

.PHONY: stage
# staging: staging-armv7 staging-armv7
stage: staging-armv7

.PHONY: staging-%
staging-%:
	mkdir -p staging/$*
	for f in `find packages -mindepth 1 -maxdepth 1 -type d -print` ; do \
	  if [ -e $$f/Makefile ]; then \
	    ${MAKE} -C $$f ARCH=$* stage || exit ; \
	  fi; \
	done

.PHONY: unstage
unstage: clobber-armv7

.PHONY: clobber-%
clobber-%:
	for f in `find packages -mindepth 1 -maxdepth 1 -type d -print` ; do \
	  if [ -e $$f/Makefile ]; then \
	    ${MAKE} -C $$f ARCH=$* clobber || exit ; \
	  fi; \
	done

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

toolchain/arm-2007q3/.unpacked: downloads/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
	mkdir -p toolchain
	tar -C toolchain -x -f $<
	touch $@

downloads/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2:
	mkdir -p downloads
	wget -O $@ http://www.codesourcery.com/sgpp/lite/arm/portal/package1787/public/arm-none-linux-gnueabi/arm-2007q3-51-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

doctors/webosdoctorp100ewwsprint-1.3.5.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pre/p135r0d12302009/sr1ntp135rod/webosdoctorp100ewwsprint.jar

doctors/webosdoctorp200ewwsprint-1.3.5.jar:
	mkdir -p doctors
	wget -O $@ http://palm.cdnetworks.net/rom/pixi/px135r0d12302009/sr1ntp135rod/webosdoctorp200ewwsprint.jar

clean:
	rm -f .*~ *~ scripts/*~ support/*~ packages/*/*~

clobber:
	rm -rf toolchain rootfs
