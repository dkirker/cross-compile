#=============
# These rules are for the toplevel makefile
#=============

#General and simple rule to update the .depends.inc file for a given package
#Note that Make will automatically try to update the files it "include"s, so
#this rule guarantees we use up-to-date depends files
%/.depends.inc: %/Makefile
	$(MAKE) -C $* .depends.inc

#This rule exists for those directories that don't have a Makefile
%/.depends.inc:
	touch $@

#=============
# Here we deal with including all the dep files
# Note that rules need to at least include $(dep_files) as a dependency
# to make sure they're up to date
#=============

#This generates a list of every package category we want
#(excluding nonworking)
package_cats = $(shell cd packages; ls |grep -v "^nonworking$$")

#This is a list of every package we want, builds on the above
package_dirs := \
	$(shell cd packages; find $(package_cats) -mindepth 1 -maxdepth 1 -type d \
	-exec [ -e {}/Makefile ] \; -print)

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

#This rule tries every package by itself after a fresh clobber
#to help draw out packages with missing deps
ifeq ("$(INC_DEPS)","")
#This rule just exists to making invoking simpler
debug:
	$(MAKE) -C . debug INC_DEPS=1 ARCH=armv7
else
debug:
	@for i in $(all_targets); do \
		$(MAKE) -C . clobber-$(ARCH) >& /dev/null; \
		rm -rf staging; \
		$(MAKE) -C . setup; >& /dev/null; \
		echo "==================================="; \
		echo "Testing package $$i..."; \
		echo "==================================="; \
		$(MAKE) -C . $$i INC_DEPS=1 ARCH=$(ARCH) || exit 1; \
	done


endif
