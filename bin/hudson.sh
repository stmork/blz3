#!/bin/bash

set -e

make configure
./configure
make depend
make 
