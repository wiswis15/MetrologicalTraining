#!/bin/bash

STAGING_DIR="$PWD/staging/usr"

export PACKAGE_NAME=playready

export EXTRA_CONF_ARGS='-DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32'
export SRC_DIR_OVERRIDE='../src/src'

./build-cmake.sh

echo "prefix=$STAGING_DIR" > staging/usr/lib/pkgconfig/playready.pc

cat patches/playready.pc >> staging/usr/lib/pkgconfig/playready.pc

