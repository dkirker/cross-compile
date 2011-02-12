#!/bin/sh

APP_DIR=$(dirname $0)
if [ "x$APP_DIR" == "x." ]
then
	APP_DIR=$(pwd)
fi

eval $($APP_DIR/pdl-helper)

export NAVIT_USER_DATADIR=$PDL_DataFilePath
export LD_LIBRARY_PATH=$APP_DIR/lib:/media/internal/.widk/usr/lib

unset LC_ALL
export LANG=${PDL_Language}.UTF-8

if [ -e $NAVIT_USER_DATADIR/lang-override ]
then
	export LANG=$(cat $NAVIT_USER_DATADIR/lang-override)
fi

export SPEECHD_SOCKET=/tmp/speechd-sock

export NAVIT_LOGFILE=$NAVIT_USER_DATADIR/navit.log

test -d $NAVIT_USER_DATADIR || mkdir -p $NAVIT_USER_DATADIR/maps

#delete log if it is more the 300kb
touch $NAVIT_LOGFILE
if [ `ls -l $NAVIT_LOGFILE | awk '{print $5}'` -gt 300000 ];then
	rm -f ${NAVIT_LOGFILE}.gz
	gzip -9 $NAVIT_LOGFILE
fi

echo "------------------------- Start Navit ----------------------------------" >> $NAVIT_LOGFILE
date >> $NAVIT_LOGFILE

# Migrate data to new directories
if test -d /media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0 
then
	mv /media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0/* $NAVIT_USER_DATADIR
	mv /media/internal/MapsNavit/* $NAVIT_USER_DATADIR/maps
	rmdir /media/internal/MapsNavit
	sed -i -re "s/\/media\/internal\/MapsNavit/\$NAVIT_USER_DATADIR\/maps/"  $NAVIT_USER_DATADIR/navit.xml
	rmdir /media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0
fi

test -e $NAVIT_USER_DATADIR/navit.xml || cp -R $APP_DIR/dist_files/* $NAVIT_USER_DATADIR/                                                                 
pgrep "^navit$" || exec $APP_DIR/bin/navit -c $NAVIT_USER_DATADIR/navit.xml 2>&1 | tee -a $NAVIT_LOGFILE

