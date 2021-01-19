#!/bin/bash

set -e

source setenv.sh

rm -rf build/thunder-tools/build
mkdir -p build/thunder-tools/build
cd build/thunder-tools/build

$CMAKE -DCMAKE_INSTALL_PREFIX=$PREFIX -DGENERIC_CMAKE_MODULE_PATH=$CMAKE_MODULE_PATH $BASE_DIR/repos/Thunder/Tools
make install

