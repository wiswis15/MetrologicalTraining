#!/bin/bash

BASE_DIR=$PWD

cd ../../staging/usr

PREFIX_DIR=$PWD

cd $BASE_DIR

rm -rf build
mkdir build
cd build

INC="-I$PREFIX_DIR/include"
LIBS="-Wl,--disable-new-dtags -Wl,-rpath,$PREFIX_DIR/lib -L$PREFIX_DIR/lib -lWPEFrameworkCore -lWPEFrameworkProtocols -lWPEFrameworkCryptalgo -lWPEFrameworkTracing -lpthread"
GXX='g++ -m32 -std=c++11 -g -O0 -fno-stack-protector'

#$GXX -c $INC ../src/proxystubs.cpp -o proxystubs.o
#$GXX -c $INC ../src/server.cpp -o server.o
#$GXX -c $INC ../src/client.cpp -o client.o

#$GXX server.o proxystubs.o -o server $LIBS
#$GXX client.o proxystubs.o -o client $LIBS

$GXX -c $INC ../src/main.cpp -o main.o
$GXX main.o -o app11 $LIBS


