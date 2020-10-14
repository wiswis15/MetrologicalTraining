#!/bin/bash

set -e

BUILDROOT_DIR=/home/sander/Projects/buildroot/rpi/buildroot
GXX=$BUILDROOT_DIR/output/host/usr/bin/arm-linux-g++
BR_INCLUDE=$BUILDROOT_DIR/output/staging/usr/include
BR_LIB=$BUILDROOT_DIR/output/staging/usr/lib
BUILD_DIR=$BUILDROOT_DIR/output/build/wpeframework-7f5e13eb4874143dbdaf9c818e8baa894b40d241/Source

CORE_O_DIR=$BUILD_DIR/core/CMakeFiles/WPEFrameworkCore.dir
TRACING_O_DIR=$BUILD_DIR/tracing/CMakeFiles/WPEFrameworkTracing.dir

export PKG_CONFIG_PATH=$BUILDROOT_DIR/output/staging/usr/lib/pkgconfig
CFLAGS=$(pkg-config --cflags WPEFrameworkCore | sed 's,\-I[^ ]*,,g')

rm -rf build.rpi
mkdir build.rpi
cd build.rpi

$GXX $CFLAGS -g -c -I$BR_INCLUDE/WPEFramework ../src/ResourceMonitorImplementation.cpp -o ResourceMonitorImplementation.o
$GXX $CFLAGS -g -c -I$BR_INCLUDE/WPEFramework ../src/main.cpp -o main.o

$GXX $CFLAGS -g $(ls $CORE_O_DIR/*.o $TRACING_O_DIR/*.o | grep -v Module.cpp.o) ResourceMonitorImplementation.o main.o -o ResourceMonitor -lpthread -lgcov -ldl

