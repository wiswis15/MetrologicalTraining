#!/bin/bash

set -e

source setenv.sh

rm -rf build/thunder-interfaces/build
mkdir -p build/thunder-interfaces/build
cd build/thunder-interfaces/build

$CMAKE -DBUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DGENERIC_CMAKE_MODULE_PATH=$CMAKE_MODULE_PATH -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH -DCMAKE_SYSROOT=$STAGING_DIR $BASE_DIR/repos/ThunderInterfaces

# TODO: why does CMake pass "--sysroot" to g++ ??
find -name 'build.make' -o -name 'link.txt' | xargs sed -i 's,\-\-sysroot[^ ]*,,g'

make -j4
make install

