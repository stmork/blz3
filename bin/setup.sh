#!/bin/tcsh

if ( ! $?BLZ3_HOME ) then
  setenv BLZ3_HOME ~blz3
endif

echo "Using Blizzard III (Home: $BLZ3_HOME)"

setenv BLZ3_INCLUDE  $BLZ3_HOME/include
setenv BLZ3_DOC      $BLZ3_HOME/doc
setenv BLZ3_MAN      $BLZ3_HOME/man
setenv BLZ3_SYSINC   $BLZ3_HOME/include_unix

setenv BLZ3_LIB      "$BLZ3_HOME/lib/$OSTYPE"_"$CPUTYPE"
setenv BLZ3_BIN      "$BLZ3_HOME/bin/$OSTYPE"_"$CPUTYPE"

setenv LIB_B3        "-lB3Raytrace -lB3Base"
setenv LIB_X3        "-lX11 -lXt"
setenv LIB_S3        "-lm -lpthread"

setenv STDINC        "$ARCHFLAG -I$BLZ3_SYSINC -I$BLZ3_INCLUDE -I$MOTIF_INCLUDE"
setenv LDFLAGS       "-L$BLZ3_LIB -L$MOTIF_LIB"

if ( $?BLZ3_USE_OPENGL ) then
  setenv LIB_OGL3    "-lglut -lGLU -lXmu -lGL"
  setenv LDFLAGS     "$LDFLAGS -L$GLUT_LIB"
  setenv STDINC      "$STDINC  -I$GLUT_INCLUDE"
else
  setenv LIB_OGL3    ""
endif

if ( `echo $PATH | fgrep $BLZ3_BIN | wc -l` == "0") then
   setenv PATH   "$BLZ3_BIN":"$PATH"
endif

if ( $?MANPATH ) then
  setenv MANPATH           $MANPATH":"$BLZ3_MAN
else
  setenv MANPATH           $BLZ3_MAN
endif
