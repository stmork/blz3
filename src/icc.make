#!/bin/tcsh
echo "Build command: "$1

setenv OFLAGS "-O3 -mcpu=pentium4 -w"
#setenv OFLAGS "-O3 -parallel -march=pentium4 -w"
#setenv OFLAGS "-O3 -tpp6 -xMiKW -w"

# Copy some options
setenv CFLAGS    "$OFLAGS"
setenv CXXFLAGS  "$CFLAGS -xM -axiKW"

# Some global vars
setenv CC iccbin
setenv GCC $CC
setenv CXX iccbin

# Files and directories
setenv SRC       "image/*.cc base/*.cc raytrace/*.cc system/*.cc system_unix/*.cc"
setenv LIBS      "$LIB_X3 $LIB_S3 $LIB_OGL3 -lB3tiff -lB3jpeg"
setenv PROF_DIR   /tmp/profdir
setenv PROF_FILES "../data/*.bwd GeoNebel.bwd AllShapes.bwd"
setenv PROF_FILES "../data/Material.bwd ../data/Shapes.bwd Vase.bwd Wassersockel.bwd AllShapes.bwd"
setenv PROF_FILES "../data/Material.bwd ../data/Shapes.bwd ../data/Lichttest.bwd"

if ( $?BLZ3_CODECOV_DIR ) then
    setenv CODECOV_DIR $BLZ3_CODECOV_DIR
  else
    setenv CODECOV_DIR "/tmp/codecoverage"
endif

# Build image file libraries
#setenv CFLAGS "-O3 -xM -w"
#setenv ASMFLAGS "-O3 -w -march=pentiumiii"
setenv ASMFLAGS "-O3 -w -march=pentium4 -ipo_S"

# Build raytracer
switch ( $1 )
   case "ipo" :
       make clean
       make -C Btiff -j 4 libB3tiff.a
       make -C Bjpeg -j 4 libB3jpeg.a
       echo "Build options: "$CXXFLAGS
       echo "Building with -ipo..."
       $CXX -ipo $CXXFLAGS -D_DEBUG $STDINC $LDFLAGS $SRC brt3/brt3.cc -o brt3/brt3 $LIBS
       breaksw

   case "asm" :
       make clean
       echo "Build options: $ASMFLAGS"
       echo "Building with -ipo..."
       $CXX $ASMFLAGS $STDINC $LDFLAGS $SRC brt3/brt3.cc
       breaksw

   case "pgo" :
       source icc.make pgo_start
       source icc.make pgo_render
       source icc.make pgo_end
       breaksw

   case "pgo_start" :
       make clean
       rm -rf $PROF_DIR
       make -C Btiff -j 4 libB3tiff.a
       make -C Bjpeg -j 4 libB3jpeg.a
       test -d $PROF_DIR || mkdir -p $PROF_DIR || exit 1
       echo "Build options: "$CFLAGS
       echo "Building with -prof_gen..."
       $CXX -prof_genx -prof_dir$PROF_DIR $CFLAGS $STDINC $LDFLAGS $SRC brt3/brt3.cc -o brt3/brt3 $LIBS
       breaksw

   case "pgo_render" :
       brt3/brt3 -a -w -n    AlleObjekte.bwd
       brt3/brt3 -w -a -n    Chair.bwd Planks.bwd Blockkugel.bwd Wassersockel.bwd
#       brt3/brt3 -w -a -n    Band.bwd
#       brt3/brt3 -w -a -n g Skyart.bwd Doku/Glas.bwd
       brt3/brt3 -w -a -n -g $PROF_FILES
       brt3/brt3 -w    -n -g ../data/Animationstest.bwd
       brt3/brt3 -w -a    -g GeoNebel.bwd VarioNebel.bwd
       breaksw

   case "pgo_end" :
       echo "Build options: "$CXXFLAGS
       echo "Building with -ipo -prof_use..."
       $CXX -ipo -prof_use -prof_dir$PROF_DIR $CXXFLAGS $STDINC $LDFLAGS $SRC brt3/brt3.cc -o brt3/brt3 $LIBS
       breaksw

   case "codecov" :
        test -d $CODECOV_DIR || mkdir $CODECOV_DIR
        echo "Building code coverage summary into: "$CODECOV_DIR
        (cd $CODECOV_DIR;umask 002;/opt/intel/compiler80/bin/codecov -dpi /tmp/profdir/pgopti.dpi -spi /tmp/profdir/pgopti.spi -counts\
             -prj "Blizzard III" -mname "Blizzard Development Group" -maddr "blz3-dev@morknet.de")
       breaksw

   case "pgo_install" :
       cp brt3/brt3 `which brt3.icc`
       breaksw
endsw
