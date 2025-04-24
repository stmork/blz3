#!/bin/bash

set -e

export BLZ3_HOME=$PWD
export LD_LIBRARY_PATH=$BLZ3_HOME/lib

export OFLAGS="-O3"
export CFLAGS=""
export CXXFLAGS=""
export CPPFLAGS=""
export JOBS=`nproc`

test -f src/Makefile && make clean
bin/clobber.sh
make config depend

cd src
qmake6 CONFIG+=debug CONFIG+=gcov -r
rm */Makefile
cd ..

make -j ${JOBS} install
make -j ${JOBS}
make install
make cppcheck

bin/brt3 -n -s 512 data/* >/dev/null

cd src/OpenGL
./qrender-sct --gtest_output=xml
cd ../..

cd src/unittest
make -j ${JOBS} valgrind
make -j ${JOBS} -f Makefile.longtest valgrind
cd ../..

make lcov
