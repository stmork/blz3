#!/bin/tcsh

set BLZ3_HOME=$1

test $BLZ3_HOME || exit 1

cd $BLZ3_HOME

echo "Cleaning files..."
if ( -f src/Makefile ) then
  make -C src clean
  rm -rf `find src -name Makefile` config.*
endif
test -f include_unix/blz3/autoconf.h && rm -f include_unix/blz3/autoconf.h
test -f configure      && rm  -f configure
test -d autom4te.cache && rm -rf autom4te.cache
test -f data.tar.gz    && rm  -f data.tar.gz

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
