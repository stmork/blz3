#!/bin/bash

export BLZ3_HOME=$PWD

make config depend

cd src
qmake CONFIG+=debug CONFIG+=lcov -r
rm */Makefile
cd ..

make -j 4 install

