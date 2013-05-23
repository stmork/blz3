#!/bin/bash

BLZ3_HOME=$PWD
BLZ3_DEBIAN=${PWD}/../debian/blz3
BLZ3_DEBIAN_DEV=${PWD}/../debian/blz3-dev
ARCH=`dpkg --print-architecture`

rm -rf ${BLZ3_DEBIAN} ${BLZ3_DEBIAN_DEV} 
umask 022

mkdir -p ${BLZ3_DEBIAN}/usr/bin
mkdir -p ${BLZ3_DEBIAN}/DEBIAN

mkdir -p ${BLZ3_DEBIAN_DEV}/usr/share/doc/blz3
mkdir -p ${BLZ3_DEBIAN_DEV}/usr/include
mkdir -p ${BLZ3_DEBIAN_DEV}/usr/lib
mkdir -p ${BLZ3_DEBIAN_DEV}/DEBIAN

export CXXFLAGS="-O3 -ffast-math -ftree-vectorize -s"
CFLAGS="${CXXFLAGS}"
unset CPPFLAGS

set -e

make configure
./configure BLZ3_DOC=${BLZ3_DEBIAN_DEV}/usr/share/doc/blz3 --prefix=${BLZ3_DEBIAN} --exec-prefix=${BLZ3_DEBIAN}/usr
make depend
make 
make install documentation
make test

sed -e s/%ARCH%/${ARCH}/g control-blz3 >${BLZ3_DEBIAN}/DEBIAN/control
dpkg -b ${BLZ3_DEBIAN} blz3.deb

sed -e s/%ARCH%/${ARCH}/g control-blz3-dev >${BLZ3_DEBIAN_DEV}/DEBIAN/control
rsync -av include/blz3/      ${BLZ3_DEBIAN_DEV}/usr/include/blz3/
rsync -av include_unix/blz3/ ${BLZ3_DEBIAN_DEV}/usr/include/blz3/
cp -a lib/lib*.a ${BLZ3_DEBIAN_DEV}/usr/lib
find  ${BLZ3_DEBIAN_DEV} -name .svn -type d | xargs rm -rf 
dpkg -b ${BLZ3_DEBIAN_DEV} blz3-dev.deb
