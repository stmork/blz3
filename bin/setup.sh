#!/bin/tcsh

echo "Using Blizzard III"

setenv BLZ3_HOME     ~blz3
setenv BLZ3_INCLUDE  $BLZ3_HOME/include
setenv BLZ3_DOC      $BLZ3_HOME/doc
setenv BLZ3_MAN      $BLZ3_HOME/man
setenv BLZ3_SYSINC   $BLZ3_HOME/include_unix

setenv BLZ3_LIB      "$BLZ3_HOME/lib/$OSTYPE"_"$CPUTYPE"
setenv BLZ3_BIN      "$BLZ3_HOME/bin/$OSTYPE"_"$CPUTYPE"

setenv LIB_B3        "-lB3System -lB3Base -lB3Raytrace -lB3Image -lB3jpeg -lB3tiff"
setenv LIB_X3        "-lX11 -lXt"
setenv LIB_S3        "-lm -lpthread"
setenv LIB_OGL3      "-lGL -lglut -lGLU -lXmu"

setenv STDINC        "$ARCHFLAG -I$BLZ3_SYSINC -I$MOTIF_INCLUDE"
setenv LDFLAGS       "-L$BLZ3_LIB -L$MOTIF_LIB"

if ( `echo $INH_PATH | fgrep $BLZ3_BIN | wc -l` == "0") then
   setenv INH_PATH   "$INH_PATH $BLZ3_BIN"
endif

if ( $?MANPATH ) then
  setenv MANPATH           $MANPATH":"$BLZ3_MAN
else
  setenv MANPATH           $BLZ3_MAN
endif
