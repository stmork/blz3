#!/bin/tcsh

if ( ! $?BLZ3_HOME ) then
  setenv BLZ3_HOME ~blz3
endif

echo "Using Blizzard III (Home: $BLZ3_HOME)"
