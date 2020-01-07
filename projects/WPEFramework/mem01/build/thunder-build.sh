#!/bin/bash

set -e

STAGING_DIR="$PWD/staging/usr"

mkdir -p $STAGING_DIR

SRC_DIR="$PWD/repos/Thunder"

rm -rf build/thunder/build
mkdir -p build/thunder/build
cd build/thunder/build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$STAGING_DIR -DCMAKE_CXX_FLAGS='-O0 -g -m32' -DCMAKE_C_FLAGS='-O0 -g -m32' -DOOMADJUST="" -DSYSTEM_PREFIX="NOS" -DVIRTUALINPUT=ON -DEXTERN_EVENTS='WebSource' -DCMAKE_STRIP='/usr/bin/echo' -DMEMORYOVERVIEW=ON $SRC_DIR
make -j4
make install
cd ..
