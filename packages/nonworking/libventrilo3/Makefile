NAME    = libventrilo
#DEPENDS = gsm
SVN_URL = http://svn.mangler.org/mangler/trunk
REVISION= 1026

.PHONY: unpack
unpack: build/.unpacked

build/.unpacked:
	mkdir -p build
	svn checkout -r ${REVISION} -q --non-interactive --depth files ${SVN_URL} build/src
	svn checkout -r ${REVISION} -q --non-interactive ${SVN_URL}/libventrilo3 build/src/libventrilo3
	svn checkout -r ${REVISION} -q --non-interactive ${SVN_URL}/m4 build/src/m4
	touch $@

update: build/.unpacked
	( cd build/src/libventrilo3 ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update -r ${REVISION} --non-interactive; \
			rm -f ../*.built ../.configure ../.aclocal; \
		fi )
	( cd build/src/m4 ; \
		if [ -n "`svn status -u | egrep -v '^Status' | egrep -v '^\?'`" ]; then \
			svn update -r ${REVISION} --non-interactive; \
			rm -f ../*.built ../.configure ../.aclocal; \
		fi )

build/.patched:
	patch -p0 build/src/configure.ac < patches/configure.ac.patch
	patch -p0 build/src/Makefile.am < patches/Makefile.am.patch
	touch $@

include ../../../support/staging.mk

include ../../../support/cross-compile.mk

stage-local:: build/${ARCH}.built

build/%.configured: build/.unpacked build/.patched
	rm -f $@
	autoreconf build/src
	cd build/src/ ; ${SB2} env LDFLAGS="-L/usr/local/lib" ./configure --disable-glibtest
	touch $@

build/%.built: build/.unpacked build/.patched build/${ARCH}.configured
	rm -f $@
	mkdir -p build/$*
	cp -R build/src/* build/$*
	( cd build/$*/libventrilo3 ; ${SB2} $(MAKE) )
	( cd build/$*/libventrilo3 ; ${SB2} $(MAKE) install )
	touch $@

clean:
	( cd build/${ARCH} ; ${SB2} $(MAKE) clean )


clobber::
	rm -rf build
