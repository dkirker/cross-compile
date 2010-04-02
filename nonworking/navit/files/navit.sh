#!/bin/sh

#unset LC_ALL
export LANG=de_DE.UTF-8
#export LC_MESSAGES=de_DE.UTF-8

export NAVIT_USER_DATADIR=/media/internal/.app-storage/file_.media.cryptofs.apps.usr.palm.applications.org.webosinternals.navit_0
#export NAVIT_LOGFILE=$NAVIT_USER_DATADIR/navit.log
export LD_LIBRARY_PATH=/media/cryptofs/apps/usr/palm/applications/org.webosinternals.navit/lib

test -d $NAVIT_USER_DATADIR || mkdir -p $NAVIT_USER_DATADIR
test -e $NAVIT_USER_DATADIR/navit.xml || cp navit.xml.dist $NAVIT_USER_DATADIR/navit.xml
#date > $NAVIT_LOGFILE

pgrep "^navit$" || exec /media/cryptofs/apps/usr/palm/applications/org.webosinternals.navit/bin/navit -c $NAVIT_USER_DATADIR/navit.xml

