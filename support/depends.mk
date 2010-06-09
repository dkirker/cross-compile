#=============
# This rule is for each package to include
#=============

#write dependencies out to a file
#(to avoid parsing them from the Makefile)
.depends: Makefile
	echo $(DEPENDS) > $@

