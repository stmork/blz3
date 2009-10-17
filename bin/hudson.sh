#!/bin/bash

make configure
./configure
make depend
make 
make documentation
