#!/bin/bash

SCRIPT_DIR=$(dirname $0)
SCRIPT_DIR="$PWD/$SCRIPT_DIR"

rm -rf staging
mkdir -p staging/usr

rm -rf build
mkdir build
cd build

BUILDOPTIONS=" \
   -DCMAKE_BUILD_TYPE=Debug \
   -DCMAKE_INSTALL_PREFIX="$PWD/../staging/usr" \
   -DINSTALL_HEADERS_TO_TARGET=ON \
   -DWPEFRAMEWORK_TEST_APPS=ON \
   -DWPEFRAMEWORK_TEST_TRACINGTEST=ON \
   -DWPEFRAMEWORK_TEST_MEASUREMENTS=ON \
   -DWPEFRAMEWORK_CRYPTALGO=ON \
   -DWPEFRAMEWORK_TRACING=ON \
   -DWPEFRAMEWORK_PROTOCOLS=ON \
   -DWPEFRAMEWORK_PROCESS=OFF \
   -DWPEFRAMEWORK_PROXY_STUB=OFF \
   -DWPEFRAMEWORK_PLUGINS_LIB=ON \
   -DWPEFRAMEWORK_INTERFACES_LIB=OFF \
   -DWPEFRAMEWORK_WPEFRAMEWORK_EXECUTABLE=OFF"

cmake $BUILDOPTIONS $SCRIPT_DIR/../..
make install

