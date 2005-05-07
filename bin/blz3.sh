#!/bin/sh

blz3data=/usr/share/blizzard

if [ -d $blz3data ]
then
  export BLZ3_DATA=$blz3data/Data
  export BLZ3_TEXTURES=$blz3data/Textures
fi

export BLZ3_PICTURES=$HOME/Blizzard/Pictures
test -d $BLZ3_PICTURES || mkdir -p $BLZ3_PICTURES
