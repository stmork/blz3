#!/bin/tcsh

cd $BLZ3_HOME

make -C src remdepend
make -C src clean

chgrp -R blz3 *
rm -f lib/*/lib*.a
rm -f lib/*/*.lib
rm -f bin/*/*

chmod 770 bin/*
chmod 775 bin/*/*
find include* lib WWW bin doc man -type d -exec chmod 775 {} \;
find include* lib WWW     doc man -type f -exec chmod 664 {} \;
find src                          -type d -exec chmod 770 {} \;
find src -name "*.c*"                     -exec chmod 660 {} \;

find $BLZ3_HOME -name "*.log" -exec rm {} \;
find $BLZ3_HOME -name "*.ncb" -exec rm {} \;
find $BLZ3_HOME -name "*.plg" -exec rm {} \;
find $BLZ3_HOME -name "*.mak" -exec rm {} \;
find $BLZ3_HOME -name "*.bak" -exec rm {} \;
find $BLZ3_HOME -name "*~"    -exec rm {} \;
find $BLZ3_HOME -name ".#*"   -exec rm {} \;
find $BLZ3_HOME -name DEADJOE -exec rm {} \;

rm -rf $BLZ3_HOME/Debug $BLZ3_HOME/Release
