#!/bin/bash

BASE_DIR=$PWD
STAGING="$PWD/staging/usr"

export PKG_CONFIG_PATH="$STAGING/lib/pkgconfig"
export LD_LIBRARY_PATH="$STAGING/lib"

mkdir -p $STAGING

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

cmake -DCMAKE_BUILD_TYPE=Debug $EXTRA_CONF_ARGS -DCMAKE_INSTALL_PREFIX=$STAGING ../src

cd $BASE_DIR

./build-make.sh

