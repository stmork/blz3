#!/bin/tcsh

cd $BLZ3_HOME

echo "Setting groups..."
chgrp -R blz3 .

echo "Setting permissions..."
chmod 770 bin/*
chmod 775 bin/*/*
find include* lib bin -type d -exec chmod 775 {} \;
find include* lib     -type f -exec chmod 664 {} \;
find src                          -type d -exec chmod 770 {} \;
find src -name "*.c*"                     -exec chmod 660 {} \;

exit 0
