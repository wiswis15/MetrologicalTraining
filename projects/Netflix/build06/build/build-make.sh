#!/bin/bash

set -e

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

mkdir -p build
cd build

BUILD_DIR=$PWD

if [[ -v MAKE_PROC_COUNT ]]; then
   EXTRA_MAKE_ARG="-j$MAKE_PROC_COUNT $EXTRA_MAKE_ARG"
fi

make $EXTRA_MAKE_ARG
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi

if [ -z "$MAKE_NO_MAKE_INSTALL" ]; then
   make install
   rc=$?
   if [[ $rc != 0 ]]; then
      exit $rc
   fi
fi

if [[ -v AFTER_MAKE_INSTALL_CMD ]]; then
   $AFTER_MAKE_INSTALL_CMD
fi

