#!/bin/bash

BUILDROOT_DIR=/home/sander/Projects/buildroot/nos/buildroot

BASE_DIR=$PWD

BUILDROOT_BIN="$BUILDROOT_DIR/output/host/usr/bin"
BUILDROOT_INC="$BUILDROOT_DIR/output/staging/usr/include"

cd $BASE_DIR

GCC="$BUILDROOT_BIN/arm-linux-gcc -fPIC"
GXX="$BUILDROOT_BIN/arm-linux-g++ -fPIC --std=c++11 -g"

rm -rf build_nos
mkdir build_nos
cd build_nos

INC="-I$BUILDROOT_INC/WPEFramework"

$GXX $INC -c ../eclipse/src/Module.cpp -o Module.o
$GXX $INC -c ../eclipse/src/Plugin01.cpp -o Plugin01.o

$GXX -shared -o libWPEFrameworkPlugin01.so Module.o Plugin01.o

