#!/bin/bash

set -e

rm -rf build staging

BASE_DIR=$PWD

PREFIX="$PWD/staging/usr"
mkdir -p $PREFIX

CMAKE_MODULE_PATH="$PWD/staging/share/cmake/Modules"
mkdir -p $CMAKE_MODULE_PATH

mkdir -p build/thunder-tools/build
cd build/thunder-tools/build

cmake -DCMAKE_INSTALL_PREFIX=$PREFIX -DGENERIC_CMAKE_MODULE_PATH=$CMAKE_MODULE_PATH $BASE_DIR/repos/Thunder/Tools
make install

cd $BASE_DIR

mkdir -p build/thunder/build
cd build/thunder/build

cmake -DBUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH -DVIRTUALINPUT=ON -DPORT=8081 -DDEADLOCK_DETECTION=ON $BASE_DIR/repos/Thunder
make -j4
make install

cd $BASE_DIR

mkdir -p build/thundernanoservices/build
cd build/thundernanoservices/build

cmake -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DPLUGIN_TRACECONTROL=ON -DPLUGIN_REMOTECONTROL=ON  $BASE_DIR/repos/ThunderNanoServices
make -j4
make install

cd $BASE_DIR

rm -rf staging/usr/share/WPEFramework/Controller/UI
mkdir -p staging/usr/share/WPEFramework/Controller/UI

cp -r repos/ThunderUI/dist/* staging/usr/share/WPEFramework/Controller/UI

cd $BASE_DIR

