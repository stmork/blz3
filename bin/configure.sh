#!/bin/tcsh

cd $BLZ3_HOME

make -C src remdepend
make -C src clean

chgrp -R blz3 *
rm -f lib/*/lib*.a
rm -f bin/*/*

chmod 770 bin/*
chmod 775 bin/*/*
find include* lib WWW bin doc man -type d -exec chmod 775 {} \;
find include* lib WWW     doc man -type f -exec chmod 664 {} \;
find src                          -type d -exec chmod 770 {} \;
find src -name "*.c*"                     -exec chmod 660 {} \;

find . -name "*.log" -exec rm {} \;

cd src

