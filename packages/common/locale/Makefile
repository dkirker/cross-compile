NAME    = locale
VERSION = 1
SRC = ../../../toolchain/arm-2007q3/arm-none-linux-gnueabi/libc/armv4t/usr/lib/locale
APP_DIR = /media/cryptofs/apps/usr/palm/applications/org.webosinternals.locale
LOCALES = cs_CZ.utf8 de_DE.utf8 en_CA.utf8 en_GB.utf8 en_US.utf8 es_MX.utf8 \
	  es_US.utf8 fr_CA.utf8 fr_FR.utf8 he_IL.utf8 zh_CN.utf8

.PHONY: stage-local

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.built: 
	rm -f $@
	mkdir -p build/${APP_DIR}
	-$(foreach locale, ${LOCALES}, \
		cp -dR ${SRC}/$(locale) build/${APP_DIR}/; \
	)
	touch $@

clean: clobber


clobber::
	rm -rf build
