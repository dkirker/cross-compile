#
# debian package 'ctags' is required to build
#

NAME    = stella
VERSION = 3.0-DEV
DEPENDS = jpeg libpng12 freetype fontconfig libsdl
SVN_URL = https://stella.svn.sourceforge.net/svnroot/stella/trunk
APP_DIR = /media/cryptofs/apps/usr/palm/applications/de.daemon.stella


.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build/src
	svn checkout -q --non-interactive ${SVN_URL} build/src
	touch $@

update: build/.unpacked
	( cd build/src ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update --non-interactive; \
			rm -f ../*.built ../.configure ../.aclocal; \
		fi )

include ../../support/staging.mk

include ../../support/cross-compile.mk

stage:: build/armv7.built


/usr/bin/ctags:
	@echo "*** debian package 'ctags' is required to build!   ***"
	@echo "*** install it as root with: apt-get install ctags ***"

build/.patch:
	cp build/src/src/gui/VideoDialog.cxx .
	sed -e 's/myFullscreenCheckbox/\/\/myFullscreenCheckbox/' VideoDialog.cxx > build/src/src/gui/VideoDialog.cxx
	rm -f VideoDialog.cxx
	touch $@

build/.configured: /usr/bin/ctags build/.patch
	rm -f $@
	( cd build/src ; ${SB2} LDFLAGS="\"-L/usr/local/lib -Wl,-rpath-link,/usr/local/lib -Wl,-rpath,/usr/local/lib\" PATH=/usr/local/bin:${PATH}" ./configure --prefix=${APP_DIR} \
		--disable-windowed --enable-sound --enable-debugger --enable-joystick --enable-static )
	touch $@

build/%.built: build/.unpacked build/.configured
	( cd build/src ; ${SB2} cp `which libtool` . )
	( cd build/src ; ${SB2} make && ${SB2} make install )
	touch $@

clean:
	( cd build/src/${NAME} ; ${SB2} make clean )


clobber::
	rm -rf build
