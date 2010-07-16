#!/bin/bash

BLZ3_HOME=$PWD

set -e

make configure
./configure
make depend
make 
