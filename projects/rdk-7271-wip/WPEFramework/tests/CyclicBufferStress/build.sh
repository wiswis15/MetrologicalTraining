#!/bin/bash

rm -rf staging
mkdir -p staging/usr

rm -rf build
mkdir build
cd build

BUILDOPTIONS="\
   -DCMAKE_BUILD_TYPE=Debug \
   -DCMAKE_INSTALL_PREFIX="$PWD/../staging/usr" \
   -DINSTALL_HEADERS_TO_TARGET=ON \
   -DWPEFRAMEWORK_TEST_APPS=ON \
   -DWPEFRAMEWORK_TEST_CYCLICBUFFERSTRESS=ON \
   -DWPEFRAMEWORK_TEST_MEASUREMENTS=ON \
   -DWPEFRAMEWORK_CRYPTALGO=OFF \
   -DWPEFRAMEWORK_TRACING=OFF \
   -DWPEFRAMEWORK_PROTOCOLS=OFF \
   -DWPEFRAMEWORK_PROCESS=OFF \
   -DWPEFRAMEWORK_PROXY_STUB=OFF \
   -DWPEFRAMEWORK_PLUGINS_LIB=OFF \
   -DWPEFRAMEWORK_INTERFACES_LIB=OFF \
   -DWPEFRAMEWORK_WPEFRAMEWORK_EXECUTABLE=OFF"

cmake $BUILDOPTIONS ../../..
make install
