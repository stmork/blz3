#!/bin/tcsh
echo "Build command: "$1
make clean

setenv CFLAGS "-O3 -parallel -xM -axiKW -mcpu=pentium4 -w"
#setenv CFLAGS "-O3 -parallel -march=pentiumiii -w"
#setenv CFLAGS "-O3 -tpp6 -xMiKW -w"

# Some global vars
setenv CC iccbin
setenv GCC $CC
setenv CXX iccbin

# Copy some options
setenv OFLAGS    "$CFLAGS"
setenv CXXFLAGS  "$CFLAGS"
setenv SRC       "image/*.cc base/*.cc raytrace/*.cc system/*.cc system_unix/*.cc"
setenv LIBS      "$LIB_X3 $LIB_S3 $LIB_OGL3 -lB3tiff -lB3jpeg"
setenv PROF_DIR   /tmp/profdir
setenv PROF_FILES "../data/*.bwd GeoNebel.bwd AllShapes.bwd"
setenv PROF_FILES "../data/Material.bwd ../data/Shapes.bwd Vase.bwd Wassersockel.bwd AllShapes.bwd"
setenv PROF_FILES "AlleObjekte.bwd ../data/Material.bwd ../data/Shapes.bwd Vase_Marble.bwd Wassersockel.bwd AllShapes.bwd"

# Build image file libraries
setenv CFLAGS "-O3 -xM -w"
setenv ASMFLAGS "-O3 -w -march=pentium4"
make -C Btiff -j 4 libB3tiff.a
make -C Bjpeg -j 4 libB3jpeg.a

# Build raytracer
switch ( $1 )
   case "ipo" :
       echo "Build options: "$CXXFLAGS
       echo "Building with -ipo..."
       $CXX -ipo $CXXFLAGS -D_DEBUG $STDINC $LDFLAGS $SRC brt3/brt3.cc -o brt3/brt3 $LIBS
       breaksw

   case "asm" :
       echo "Build options: $ASMFLAGS"
       echo "Building with -ipo..."
       $CXX -ipo $ASMFLAGS $STDINC $LDFLAGS $SRC brt3/brt3.cc -S
       breaksw

   case "pgo" :
       test -d $PROF_DIR || mkdir -p $PROF_DIR || exit 1
       echo "Build options: "$CFLAGS
       echo "Building with -prof_gen..."
       $CXX -prof_gen -prof_dir$PROF_DIR $CFLAGS $STDINC $LDFLAGS $SRC brt3/brt3.cc -o brt3/brt3 $LIBS
       brt3/brt3 -a -w -n $PROF_FILES
       brt3/brt3 -a -w    GeoNebel.bwd VarioNebel.bwd
       echo "Build options: "$CXXFLAGS
       echo "Building with -ipo -prof_use..."
       $CXX -ipo -prof_use -prof_dir$PROF_DIR $CXXFLAGS $STDINC $LDFLAGS $SRC brt3/brt3.cc -o brt3/brt3 $LIBS
       rm -rf $PROF_DIR
       breaksw

   case "test" :
       test -d $PROF_DIR || mkdir -p $PROF_DIR || exit 1
       echo "Build options: "$CFLAGS
       echo "Building with -prof_gen..."
       $CXX -prof_gen -prof_dir$PROF_DIR $CFLAGS $STDINC $LDFLAGS $SRC brt3/brt3.cc -o brt3/brt3 $LIBS
       breaksw
endsw
