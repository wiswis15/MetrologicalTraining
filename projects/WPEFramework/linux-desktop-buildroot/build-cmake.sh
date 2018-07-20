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

cmake $EXTRA_CONF_ARGS -DCMAKE_INSTALL_PREFIX=$STAGING -DCMAKE_BUILD_TYPE=Debug ../src

cd $BASE_DIR

./build-make.sh

