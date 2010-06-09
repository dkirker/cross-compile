#=============
# These rules are for the toplevel makefile
#=============

#Generate a rule in the form
#build_dev-sdl/libsdl: build_common/libnapp build_common/piranha (etc)
#and stash it in the .depends.inc for a given directory
packages/%/.depends.inc: packages/%/Makefile packages/%/.depends
	echo build_$*:: $(addprefix build_,$(shell cat packages/$*/.depends)) > $@

#General and simple rule to update the .depends file for a given package
%/.depends: %/Makefile
	make -C $* .depends

#This rule expects to be called on things like
#build_dev-sdl/libsdl
#Note that the dependencies are dealt with via including those
#.depends.inc files, here we have the simple task of building it
build_%::
	make -C packages/$* ARCH=$(ARCH)

#Fix a small thing I'm too lazy to fix now; surely some playing with
#the various shell invocations will make this go away
build_./%::  build_%
	echo "Built $*!"



#=============
# Here we deal with including all the dep files
# Note that rules need to at least include $(dep_files) as a dependency
# to make sure they're up to date
#=============

#This generates a list of every package category we want
#(excluding nonworking)
package_cats = $(shell cd packages; find . -mindepth 1 -maxdepth 1 -type d -not -name nonworking -print)

#This is a list of every package we want, builds on the above
package_dirs := $(shell cd packages; find $(package_cats) -mindepth 1 -maxdepth 1 -type d -print)

#same as above, with with the packages/ prefix
build_dirs := $(addprefix packages/,$(package_dirs))

#List of every .depends.inc file
dep_files = $(addsuffix /.depends.inc,$(build_dirs))

#Target for every single package
all_targets = $(addprefix build_,$(package_dirs))

#And, after all that fun has been had, finally we include the depeneds rules
#we created and now we just have to invoke them as required
include $(dep_files)

#Rule to build every package using the dependencies we've created
build_all: $(all_targets)
