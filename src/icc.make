#!/bin/tcsh
make clean

setenv CFLAGS "-O3 -parallel -xM -axiK -mcpu=pentium4 -w"
#setenv CFLAGS "-O3 -parallel -march=pentiumiii -w"
#setenv CFLAGS "-O3 -tpp6 -xMiKW -w"

# Some global vars
setenv CC iccbin
setenv GCC $CC
setenv CXX iccbin

# Copy some options
setenv OFLAGS   "$CFLAGS"
setenv CXXFLAGS "$CFLAGS"

# Build image file libraries
make -C Btiff -j 4 libB3tiff.a
make -C Bjpeg -j 4 libB3jpeg.a

# Build raytracer
echo "Build options: "$CFLAGS
$CXX -ipo $CXXFLAGS $STDINC $LDFLAGS image/*.cc base/*.cc raytrace/*.cc system/*.cc system_unix/*.cc brt3/brt3.cc -o brt3/brt3 $LIB_X3 $LIB_S3 $LIB_OGL3 -lB3tiff -lB3jpeg
