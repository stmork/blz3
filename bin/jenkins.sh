#!/bin/bash

export BLZ3_HOME=$PWD
export BLZ3_DOC=$BLZ3_HOME/doc
#env

set -e

make clean configure
./configure
make depend
make 
