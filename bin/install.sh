#!/bin/csh

if ( $?MACHTYPE ) then
  setenv INSTALL_ARCH $MACHTYPE
else
  setenv INSTALL_ARCH `uname -p`
  test $INSTALL_ARCH = "unknown" && INSTALL_ARCH `uname -m`
endif

setenv INSTALL_BIN  "brt3 bimg3 render"

if ( -d $HOME/bin/$INSTALL_ARCH ) then
  setenv INSTALL_DIR $HOME/bin/$INSTALL_ARCH
else
  test -d $HOME/bin && setenv INSTALL_DIR $HOME/bin
endif

if ( $?INSTALL_DIR ) then
  echo "Installing into $INSTALL_DIR"
  foreach bin ( $INSTALL_BIN )
    cp $BLZ3_BIN/$bin $INSTALL_DIR
  end
  unsetenv INSTALL_DIR
endif

unsetenv INSTALL_ARCH
