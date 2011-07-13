SB2 = sb2 -t ${ARCH} \
	  -M ${MAPPING_FILE} \
	  PATH=/usr/local/bin:$$PATH  \
	  PKG_CONFIG_LIBDIR=/usr/local/lib/pkgconfig:/usr/local/share/pkgconfig
