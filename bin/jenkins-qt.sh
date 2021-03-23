#!/bin/bash

set -e

export BLZ3_HOME=$PWD
export LD_LIBRARY_PATH=$BLZ3_HOME/lib

export OFLAGS="-O2"
export CFLAGS=""
export CXXFLAGS=""
export CPPFLAGS=""
export JOBS=`getconf _NPROCESSORS_ONLN`

make config depend

cd src
qmake CONFIG+=debug CONFIG+=gcov -r
rm */Makefile
cd ..

make -j ${JOBS} install
make -j ${JOBS}
make install
make cppcheck

bin/brt3 -n -s 512 data/*
cd src/unittest
make valgrind
cd ../..

make lcov
