#!/bin/tcsh

cd $BLZ3_HOME

make -C src remdepend
make -C src clean

echo "Setting groups..."
chgrp -R blz3 .

rm -f bin/*_*/* lib/*_*/* || echo "No files to delete"

echo "Setting permissions..."
chmod 770 bin/*
chmod 775 bin/*/*
find include* lib bin -type d -exec chmod 775 {} \;
find include* lib     -type f -exec chmod 664 {} \;
find src                          -type d -exec chmod 770 {} \;
find src -name "*.c*"                     -exec chmod 660 {} \;

echo "Removing temp files..."
find $BLZ3_HOME -name "*.log" -exec rm -f {} \;
find $BLZ3_HOME -name "*.ncb" -exec rm -f {} \;
find $BLZ3_HOME -name "*.plg" -exec rm -f {} \;
find $BLZ3_HOME -name "*.mak" -exec rm -f {} \;
find $BLZ3_HOME -name "*.bak" -exec rm -f {} \;
find $BLZ3_HOME -name "*~"    -exec rm -f {} \;
find $BLZ3_HOME -name ".#*"   -exec rm -f {} \;
find $BLZ3_HOME -name DEADJOE -exec rm -f {} \;

test -d $BLZ3_HOME/Debug   && rm -rf $BLZ3_HOME/Debug
test -d $BLZ3_HOME/Release && rm -rf $BLZ3_HOME/Release

exit 0
