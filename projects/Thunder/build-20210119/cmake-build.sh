#!/bin/bash

set -e

BASE_DIR=$PWD

PREFIX="$PWD/staging/usr"
mkdir -p $PREFIX

rm -rf build/cmake
mkdir -p build/cmake/src
cd build/cmake/src

tar xf ../../../zips/cmake.tgz --strip 1

cd ..
mkdir build
cd build

../src/configure --prefix=$PREFIX
make -j4
make install

