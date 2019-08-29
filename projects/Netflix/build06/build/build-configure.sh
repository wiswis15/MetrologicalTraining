#!/bin/bash

set -e

BASE_DIR=$PWD

STAGING="$PWD/staging/usr"

export PKG_CONFIG_PATH="$STAGING/lib/pkgconfig"
export LD_LIBRARY_PATH="$STAGING/lib"

cd build/$PACKAGE_NAME
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi

if [ -v IN_SRC_BUILD ]; then
   cd src
else
   rm -rf build
   mkdir build
   cd build
fi

BUILD_DIR=$PWD

if [ -v PRE_CONF_CMD ]; then
   $PRE_CONF_CMD
   cd $BUILD_DIR
fi

CONF_LOC='../src/configure'

if [ -v CONF_OVERRIDE_LOC ]; then
   CONF_LOC=$CONF_OVERRIDE_LOC
fi

export CFLAGS="-m32 -I$STAGING/include"
export CXXFLAGS="-m32 -I$STAGING/include"
export LDFLAGS="-m32 -L$STAGING/lib"

$CONF_LOC $EXTRA_CONF_ARGS --prefix=$STAGING
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi

if [ -v AFTER_CONF_CMD ]; then
   $AFTER_CONF_CMD
   cd $BUILD_DIR
fi

cd $BASE_DIR

./build-make.sh

