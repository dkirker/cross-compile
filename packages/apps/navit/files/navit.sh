#!/bin/sh

eval $(./pdl-helper)

APP_DIR=$(dirname $0)
export NAVIT_USER_DATADIR=$PDL_DataFilePath
export LD_LIBRARY_PATH=$APP_DIR/lib

unset LC_ALL
export LANG=${PDL_Language}.UTF-8
export ESPEAK_VOICE=${LANG%%_*}
export ESPEAK_SINK=pnavigation

export NAVIT_LOGFILE=$NAVIT_USER_DATADIR/navit.log
#delete log if it is more the 300kb
if [ `ls -l $NAVIT_LOGFILE | awk '{print $5}'` -gt 300000 ];then
	rm -f ${NAVIT_LOGFILE}.gz
	gzip -9 $NAVIT_LOGFILE
fi

echo "------------------------- Start Navit ----------------------------------" >> $NAVIT_LOGFILE
date >> $NAVIT_LOGFILE

test -d $NAVIT_USER_DATADIR || mkdir -p $NAVIT_USER_DATADIR

# Migrate data to new directories
if test -d /media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0 
then
	mv /media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0/* $NAVIT_USER_DATADIR
	mv /media/internal/MapsNavit $NAVIT_USER_DATADIR/maps
	sed -i -re "s/\/media\/internal\/MapsNavit/\$NAVIT_USER_DATADIR\/maps/"  $NAVIT_USER_DATADIR/navit.xml
	rmdir /media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0
fi

test -e $NAVIT_USER_DATADIR/navit.xml || cp $APP_DIR/dist_files/navit*.xml $NAVIT_USER_DATADIR/                                                                 
pgrep "^navit$" || exec $APP_DIR/bin/navit -c $NAVIT_USER_DATADIR/navit.xml

