#!/bin/bash

set -e

source setenv.sh

rm -rf build/thunder/build
mkdir -p build/thunder/build
cd build/thunder/build

$CMAKE -DBUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH -DVIRTUALINPUT=ON -DPORT=8081 -DDEADLOCK_DETECTION=ON $BASE_DIR/repos/Thunder

make -j4
make install

