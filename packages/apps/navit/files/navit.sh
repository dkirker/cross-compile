#!/bin/sh

APP_DIR=$(dirname $0)
if [ "x$APP_DIR" == "x." ]
then
	APP_DIR=$(pwd)
fi

eval $($APP_DIR/pdl-helper)

export NAVIT_USER_DATADIR=$PDL_DataFilePath
export LD_LIBRARY_PATH=$APP_DIR/lib:/media/internal/.local/lib

unset LC_ALL
export LANG=${PDL_Language}.UTF-8
export SPEECHD_SOCKET=/tmp/speechd-sock

export NAVIT_LOGFILE=$NAVIT_USER_DATADIR/navit.log
#delete log if it is more the 300kb
if [ `ls -l $NAVIT_LOGFILE | awk '{print $5}'` -gt 300000 ];then
	rm -f ${NAVIT_LOGFILE}.gz
	gzip -9 $NAVIT_LOGFILE
fi

echo "------------------------- Start Navit ----------------------------------" >> $NAVIT_LOGFILE
date >> $NAVIT_LOGFILE

test -d $NAVIT_USER_DATADIR || mkdir -p $NAVIT_USER_DATADIR

test -e $NAVIT_USER_DATADIR/navit.xml || cp $APP_DIR/dist_files/navit*.xml $NAVIT_USER_DATADIR/                                                                 
pgrep "^navit$" || exec $APP_DIR/bin/navit -c $NAVIT_USER_DATADIR/navit.xml

