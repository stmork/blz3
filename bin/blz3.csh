#!/bin/csh

set blz3data=/usr/share/blizzard

if ( -d $blz3data ) then
  setenv BLZ3_DATA     $blz3data/Data
  setenv BLZ3_TEXTURES $blz3data/Textures
endif

setenv BLZ3_PICTURES $HOME/Blizzard/Pictures
test -d $BLZ3_PICTURES || mkdir -p $BLZ3_PICTURES
