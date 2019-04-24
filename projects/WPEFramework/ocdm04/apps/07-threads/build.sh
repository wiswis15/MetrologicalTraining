#!/bin/bash

#!/bin/bash

BASE_DIR=$PWD

cd ../../staging/usr

PREFIX_DIR=$PWD

cd $BASE_DIR

rm -rf build
mkdir build
cd build

INC="-I$PREFIX_DIR/include -I$PREFIX_DIR/include/WPEFramework"
LIBS="-L$PREFIX_DIR/lib -lWPEFrameworkCore -lWPEFrameworkProtocols -lWPEFrameworkCryptalgo -lWPEFrameworkTracing -Wl,-rpath,$PREFIX_DIR/lib"
GXX='g++ -m32 -std=c++11 -g -O0'

$GXX -c $INC ../eclipse/src/main.cpp -o main.o

$GXX main.o -o threads07 $LIBS

