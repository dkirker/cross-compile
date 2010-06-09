#=============
# This is for each package to include
# Generates the depends include for top-level make
# This avoids having to parse them out, let make do it for you
#=============

#Figure out where we are.
#(This could be better)
PKG_NAME = $(shell basename $(shell pwd))
PKG_CAT = $(shell basename $(shell cd ..;pwd))
#Gets something like 'common/bison'
PKG_PATH = $(PKG_CAT)/$(PKG_NAME)

#build rules for this package
#build_common/bison: <deps>
#and
#clobber_common/bison:
.depends.inc: Makefile
	@echo "Creating dep file for $(PKG_PATH)..."
	@echo "build_$(PKG_PATH): $(addprefix build_,$(DEPENDS))" > $@
	@echo '	$$(MAKE) -C packages/$(PKG_PATH) stage ARCH=$$(ARCH) ALREADY_BUILT_DEPS=1' >> $@
	@echo >> $@
	@echo "clobber_$(PKG_PATH): " >> $@
	@echo '	$$(MAKE) -C packages/$(PKG_PATH) clobber ARCH=$$(ARCH)' >> $@

#This allows us to still run 'make stage' from a particular package directory
#and have it build the dependencies just for that package (and the package itself)
ifeq ("$(ALREADY_BUILT_DEPS)","")
stage::
	@$(MAKE) -C ../../../ ARCH=$(ARCH) INC_DEPS=1 build_$(PKG_PATH)
endif
