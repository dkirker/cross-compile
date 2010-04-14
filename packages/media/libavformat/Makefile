NAME = libavformat

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

stage::
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e ${NAME}.so ] || ln -f -s ${NAME}.so.52.24.1 ${NAME}.so )

clobber::
	rm -rf ${STAGING_DIR}/usr/lib/${NAME}.so
