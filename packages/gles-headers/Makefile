NAME = gles-headers
VERSION = 0.0.1
DEPENDS = 

DIRS = EGL GLES GLES2 KHR

include ../../support/staging.mk

stage::
	$(foreach dir,${DIRS}, \
		mkdir -p ${STAGING_DIR}/usr/include/${dir} && \
		install -m 644 include/${dir}/*.h ${STAGING_DIR}/usr/include/${dir}; \
	)

clobber::
	$(foreach dir,${DIRS}, \
		rm -f ${STAGING_DIR}/usr/include/${dir}/*.h; \
		rmdir ${STAGING_DIR}/usr/include/${dir}; \
	)

