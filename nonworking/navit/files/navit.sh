#!/bin/sh

export APP_DIR=/media/cryptofs/apps/usr/palm/applications/org.webosinternals.navit
export NAVIT_USER_DATADIR=/media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0
export LD_LIBRARY_PATH=$APP_DIR/lib

unset LC_ALL
export LANG=C
export ESPEAK_VOICE=en
export ESPEAK_SINK=pnavigation

#export NAVIT_LOGFILE=$NAVIT_USER_DATADIR/navit.log
#date > $NAVIT_LOGFILE

test -d $NAVIT_USER_DATADIR || mkdir -p $NAVIT_USER_DATADIR
test -e $NAVIT_USER_DATADIR/navit.xml || cp $APP_DIR/dist_files/navit*.xml $NAVIT_USER_DATADIR/                                                                 
pgrep "^navit$" || exec $APP_DIR/bin/navit -c $NAVIT_USER_DATADIR/navit.xml

