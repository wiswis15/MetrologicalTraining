#!/bin/bash

rm -rf build
mkdir build
cd build

g++ -g -m32 -I../../../staging/usr/include/WPEFramework ../src/main.cpp -o encrypt ../../../staging/usr/lib/libWPEFrameworkCore.so -lcrypto

