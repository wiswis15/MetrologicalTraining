#!/bin/bash

BASE_DIR=$PWD

cd ../../staging/usr

PREFIX_DIR=$PWD

cd $BASE_DIR

rm -rf build
mkdir build
cd build

INC="-I$PREFIX_DIR/include"
LIBS="-Wl,-rpath,$PREFIX_DIR/lib -L$PREFIX_DIR/lib -lWPEFrameworkCore -lWPEFrameworkProtocols -lWPEFrameworkCryptalgo -lWPEFrameworkTracing"
GXX='g++ -m32 -std=c++11 -g -O0'

$GXX -c $INC ../eclipse/src/proxystubs.cpp -o proxystubs.o
$GXX -c $INC ../eclipse/src/server.cpp -o server.o
$GXX -c $INC ../eclipse/src/client.cpp -o client.o

$GXX server.o proxystubs.o -o server $LIBS
$GXX client.o proxystubs.o -o client $LIBS

