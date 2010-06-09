#=============
# These rules are for the toplevel makefile
#=============

#General and simple rule to update the .depends.inc file for a given package
#Note that Make will automatically try to update the files it "include"s, so
#this rule guarantees we use up-to-date depends files
%/.depends.inc: %/Makefile
	$(MAKE) -C $* .depends.inc


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
