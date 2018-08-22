#!/bin/bash

#!/bin/bash

BUILDROOT_DIR=/home/sander/Projects/buildroot/nos/buildroot.master
HOST_DIR="$BUILDROOT_DIR/output/host"
HOST_BIN_DIR="$HOST_DIR/usr/bin"
HOST_LIB_DIR="$HOST_DIR/usr/lib"
STAGING_INC_DIR="$BUILDROOT_DIR/output/staging/usr/include"
STAGING_LIB_DIR="$BUILDROOT_DIR/output/staging/usr/lib"

GXX="$HOST_BIN_DIR/arm-linux-g++ -std=c++11 -g -O0 -fPIC"

INC="-I$STAGING_INC_DIR -I$STAGING_INC_DIR/WPEFramework"

rm -rf build
mkdir build
cd build

#$GXX -c $INC ../AccessorOCDM.cpp -o AccessorOCDM.o
$GXX -c $INC ../main.cpp -o main.o
#$GXX -c $INC ../ProxyStubs.cpp -o ProxyStubs.o

#$GXX -g ProxyStubs.o open_cdm.o main.o -o ocdm02 -lWPEFrameworkCore -lWPEFrameworkProtocols

#$GXX -shared AccessorOCDM.o ProxyStubs.o -o libWPEFrameworkOCDM.so

#$GXX main.o -o ocdm03 -lWPEFrameworkCore -lWPEFrameworkProtocols libWPEFrameworkOCDM.so

$GXX main.o -o ocdmtestapp01 -L$STAGING_LIB_DIR -lWPEFrameworkCore -lWPEFrameworkProtocols -lWPEFrameworkOCDM



