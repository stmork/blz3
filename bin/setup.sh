#!/bin/tcsh

if ( ! $?BLZ3_HOME ) then
  setenv BLZ3_HOME ~blz3
endif

echo "Using Blizzard III (Home: $BLZ3_HOME)"

setenv BLZ3_INCLUDE  $BLZ3_HOME/include
setenv BLZ3_DOC      $BLZ3_HOME/doc
setenv BLZ3_MAN      $BLZ3_HOME/man
setenv BLZ3_WWW      $BLZ3_HOME/WWW
setenv BLZ3_SYSINC   $BLZ3_HOME/include_unix

setenv BLZ3_LIB      "$BLZ3_HOME/lib/$OSTYPE"_"$CPUTYPE"
setenv BLZ3_BIN      "$BLZ3_HOME/bin/$OSTYPE"_"$CPUTYPE"

setenv LIB_B3        "-lB3Raytrace -lB3Base"
setenv LIB_X3        "-lX11 -lXt"
setenv LIB_S3        "-lm -lpthread"

setenv STDINC        "$ARCHFLAG -I$BLZ3_SYSINC -I$BLZ3_INCLUDE"

test -d $BLZ3_LIB || mkdir -p $BLZ3_LIB
test -d $BLZ3_BIN || mkdir -p $BLZ3_BIN
test -d $BLZ3_DOC || mkdir -p $BLZ3_DOC
test -d $BLZ3_MAN || mkdir -p $BLZ3_MAN
test -d $BLZ3_WWW || mkdir -p $BLZ3_WWW

if ( $?LDFLAGS ) then
  setenv LDFLAGS     "$LDFLAGS -L$BLZ3_LIB"
else
  setenv LDFLAGS     "-L$BLZ3_LIB"
endif

if ( $?MOTIF_INCLUDE ) then
  setenv STDINC      "$STDINC -I$MOTIF_INCLUDE"
endif
if ( $?MOTIF_LIB ) then
  setenv LDFLAGS     "$LDFLAGS -L$MOTIF_LIB"
endif

if ( $?BLZ3_USE_OPENGL ) then
  setenv LIB_OGL3    "-lglut -lGLU -lXmu -lGL"
  if ( $?GLUT_LIB) then
    setenv LDFLAGS     "$LDFLAGS -L$GLUT_LIB"
  endif
  if ( $?GLUT_INCLUDE ) then
    setenv STDINC "$STDINC -I$GLUT_INCLUDE"
  endif
else
  setenv LIB_OGL3    ""
endif

if ( ! $?INH_PATH ) then
  if ( `echo $PATH | fgrep $BLZ3_BIN | wc -l` == "0") then
     setenv PATH   "$BLZ3_BIN":"$PATH"
  endif
  else
  if ( `echo $INH_PATH | fgrep $BLZ3_BIN | wc -l` == "0") then
     setenv INH_PATH   "$BLZ3_BIN $INH_PATH"
  endif
endif

if ( $?MANPATH ) then
  setenv MANPATH           $MANPATH":"$BLZ3_MAN
else
  setenv MANPATH           $BLZ3_MAN
endif
