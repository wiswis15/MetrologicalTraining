#!/bin/bash

set -e

source setenv.sh

rm -rf build/thundernanoservices/build
mkdir -p build/thundernanoservices/build
cd build/thundernanoservices/build

# TODO: enable more plugins
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DPLUGIN_REMOTECONTROL=ON  $BASE_DIR/repos/ThunderNanoServices
make -j4
make install

