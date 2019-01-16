#!/bin/bash

BASE_DIR=$PWD
STAGING="$PWD/staging/usr"

export PKG_CONFIG_PATH="$STAGING/lib/pkgconfig"
export LD_LIBRARY_PATH="$STAGING/lib"

mkdir -p $STAGING

mkdir -p build/$PACKAGE_NAME
cd build/$PACKAGE_NAME
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi

rm -rf build
mkdir build
cd build

BUILD_DIR=$PWD

cd $BUILD_DIR

SRC_DIR='../src'
if [[ -v SRC_DIR_OVERRIDE ]]; then
   SRC_DIR=$SRC_DIR_OVERRIDE
fi

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32 $EXTRA_CONF_ARGS -DCMAKE_INSTALL_PREFIX=$STAGING $SRC_DIR
#cmake -DCMAKE_BUILD_TYPE=Debug $EXTRA_CONF_ARGS -DCMAKE_INSTALL_PREFIX=$STAGING $SRC_DIR

cd $BASE_DIR

./build-make.sh

