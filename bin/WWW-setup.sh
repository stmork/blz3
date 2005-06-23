#!/bin/sh

umask 002

cd ~blz3
chgrp -R blz3 WWW
find WWW -type d | xargs chmod 775
find WWW -type f |xargs chmod 664

cd WWW/lines/small
cp ../normal/dlg_*jpg ../normal/window_*jpg  .
find . -name "dlg_*.jpg" -exec bconv3 -i {} -f -s 0.4 -j {} \;
find . -name "window_*.jpg" -exec bconv3 -f -s 0.2 -i {} -j {} \;
