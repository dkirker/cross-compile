LIBS = $(shell grep /usr/lib ${IPKG_FILES_LIST})

stage-local:: $(foreach LIB,${LIBS},${STAGING_DIR}${LIB})
ifeq (${LIBS},)
	$(error "No libraries found in ${IPKG_FILES_LIST}")
endif

${STAGING_DIR}/% : ${ROOTFS_DIR}/%
	mkdir -p $(@D)
	rm -f $@
	cp -pR $< $@

clobber::
	rm -f $(foreach LIB,${LIBS},${STAGING_DIR}${LIB})
