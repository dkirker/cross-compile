NAME = libavcodec

DEPENDS = media/libfaac

include ../../../support/staging.mk

include ../../../support/stage-rootfs.mk

stage-local::
	( cd ${STAGING_DIR}/usr/lib ; \
	  [ -e ${NAME}.so ] || ln -f -s ${NAME}.so.52.48.0 ${NAME}.so )

clobber::
	rm -rf ${STAGING_DIR}/usr/lib/${NAME}.so

download::
