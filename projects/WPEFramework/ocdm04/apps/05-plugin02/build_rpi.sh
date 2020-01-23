#!/bin/bash

set -e

BUILDROOT_DIR=/home/sander/Projects/buildroot/rpi/buildroot

BASE_DIR=$PWD

BUILDROOT_BIN="$BUILDROOT_DIR/output/host/usr/bin"
BUILDROOT_INC="$BUILDROOT_DIR/output/staging/usr/include"

cd $BASE_DIR

GCC="$BUILDROOT_BIN/arm-linux-gcc -fPIC"
GXX="$BUILDROOT_BIN/arm-linux-g++ -fPIC --std=c++11 -g"

rm -rf build_rpi
mkdir build_rpi
cd build_rpi

INC="-I$BUILDROOT_INC/WPEFramework"

$GXX $INC -c ../eclipse/src/Module.cpp -o Module.o
$GXX $INC -c ../eclipse/src/Plugin02.cpp -o Plugin02.o

$GXX -shared -o libWPEFrameworkPlugin02.so Module.o Plugin02.o -lcompositorclient -lEGL -lGLESv2

