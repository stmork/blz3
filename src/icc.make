#!/bin/tcsh
make clean

setenv CFLAGS "-O3 -xM -axiK -mcpu=pentiumpro -w"
#setenv CFLAGS "-O3 -march=pentiumiii -w"
#setenv CFLAGS "-O3 -march=pentium4 -w"

make -C Btiff -j 4 libB3tiff.a
make -C Bjpeg -j 4 libB3jpeg.a

iccbin -ipo $CFLAGS $STDINC $LDFLAGS image/*.cc base/*.cc raytrace/*.cc system/*.cc system_unix/*.cc brt3/brt3.cc -o brt3/brt3 $LIB_X3 $LIB_S3 $LIB_OGL3 -lB3tiff -lB3jpeg
