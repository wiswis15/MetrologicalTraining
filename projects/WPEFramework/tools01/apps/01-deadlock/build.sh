#!/bin/bash

set -e

BASE_DIR=$PWD

cd ../../staging/usr/lib/pkgconfig

export PKG_CONFIG_PATH=$PWD

cd $BASE_DIR

rm -rf build
mkdir build
cd build

g++ -m32 -c -g -O0 `pkg-config --cflags WPEFrameworkCore` ../src/main.cpp -o main.o
g++ -m32 -g main.o -o app01 `pkg-config --libs WPEFrameworkCore` -pthread



