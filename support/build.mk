#=============
# These rules are for the toplevel makefile
#=============

#Generate a rule in the form
#build_dev-sdl/libsdl: build_common/libnapp build_common/piranha (etc)
#and stash it in the .depends.inc for a given directory
#also generates clobber rule
packages/%/.depends.inc: packages/%/Makefile packages/%/.depends
	echo "build_$*: $(addprefix build_,$(shell cat packages/$*/.depends))" > $@
	echo '	$$(MAKE) -C packages/$* stage ARCH=$$(ARCH)' >> $@
	echo >> $@
	echo "clobber_$*: " > $@
	echo '	$$(MAKE) -C packages/$* clobber ARCH=$$(ARCH)' >> $@

#General and simple rule to update the .depends file for a given package
%/.depends: %/Makefile
	$(MAKE) -C $* .depends


#=============
# Here we deal with including all the dep files
# Note that rules need to at least include $(dep_files) as a dependency
# to make sure they're up to date
#=============

#This generates a list of every package category we want
#(excluding nonworking)
package_cats = $(shell cd packages; ls |grep -v "^nonworking$$")

#This is a list of every package we want, builds on the above
package_dirs := $(shell cd packages; find $(package_cats) -mindepth 1 -maxdepth 1 -type d -print)

#same as above, with with the packages/ prefix
build_dirs := $(addprefix packages/,$(package_dirs))

#List of every .depends.inc file
dep_files = $(addsuffix /.depends.inc,$(build_dirs))

#Target for every single package
all_targets = $(addprefix build_,$(package_dirs))

#all the clobber targets
clobber_targets = $(addprefix clobber_,$(package_dirs))

#And, after all that fun has been had, finally we include the depeneds rules
#we created and now we just have to invoke them as required
ifneq ("$(INC_DEPS)","")
include $(dep_files)
endif
#Don't delete them, make!
.PRECIOUS: $(dep_files)

#Rule to build every package using the dependencies we've created
buildall: $(all_targets)
clobberall: $(clobber_targets)

#This rule expects to be called on things like
#build_dev-sdl/libsdl
#Note that the dependencies are dealt with via including those
#.depends.inc files, here we have the simple task of building it
build_%:
