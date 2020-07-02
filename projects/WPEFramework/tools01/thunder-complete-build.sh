#!/bin/bash

set -e

BASE_DIR=$PWD

PREFIX="$PWD/staging/usr"
mkdir -p $PREFIX

CMAKE_MODULE_PATH="$PWD/staging/share/cmake/Modules"
mkdir -p $CMAKE_MODULE_PATH

export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

rm -rf build/thunder-tools/build
mkdir -p build/thunder-tools/build
cd build/thunder-tools/build

cmake -DCMAKE_INSTALL_PREFIX=$PREFIX -DGENERIC_CMAKE_MODULE_PATH=$CMAKE_MODULE_PATH $BASE_DIR/repos/Thunder/Tools
make install

cd $BASE_DIR

rm -rf build/thunder/build
mkdir -p build/thunder/build
cd build/thunder/build

#cmake -DBUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH -DVIRTUALINPUT=ON -DPORT=8081 -DDEADLOCK_DETECTION=ON -DCRYPTOGRAPHY=ON -DCRYPTOGRAPHY_IMPLEMENTATION=OpenSSL -DOPENSSL_CRYPTO_LIBRARY=$PREFIX/lib/libcrypto.a -DOPENSSL_SSL_LIBRARY=$PREFIX/lib/libssl.a -DOPENSSL_INCLUDE_DIR=$PREFIX/include -DBUILD_TESTS=ON -DBUILD_NETFLIX_VAULT_GENERATOR=ON -DBUILD_CRYPTOGRAPHY_TESTS=ON -DBUILD_TESTS=ON $BASE_DIR/repos/Thunder
cmake -DBUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH -DVIRTUALINPUT=ON -DPORT=8081 -DDEADLOCK_DETECTION=ON -DCRYPTOGRAPHY=ON -DCRYPTOGRAPHY_IMPLEMENTATION=OpenSSL -DOPENSSL_CRYPTO_LIBRARY=$PREFIX/lib/libcrypto.a -DOPENSSL_SSL_LIBRARY=$PREFIX/lib/libssl.a -DOPENSSL_INCLUDE_DIR=$PREFIX/include -DBUILD_TESTS=ON $BASE_DIR/repos/Thunder

make -j4
make install

cd $BASE_DIR

rm -rf build/thundernanoservices/build
mkdir -p build/thundernanoservices/build
cd build/thundernanoservices/build

cmake -DCMAKE_CXX_FLAGS='-m32' -DCMAKE_C_FLAGS='-m32' -DCMAKE_INSTALL_PREFIX=$PREFIX -DPLUGIN_TRACECONTROL=ON -DPLUGIN_REMOTECONTROL=ON  $BASE_DIR/repos/ThunderNanoServices
make -j4
make install

cd $BASE_DIR

rm -rf staging/usr/share/WPEFramework/Controller/UI
mkdir -p staging/usr/share/WPEFramework/Controller/UI

cp -r repos/ThunderUI/dist/* staging/usr/share/WPEFramework/Controller/UI

