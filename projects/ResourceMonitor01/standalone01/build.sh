#!/bin/bash

set -e

BUILDROOT_DIR=/home/sander/Projects/buildroot/qemu-arm/buildroot
GXX=$BUILDROOT_DIR/output/host/usr/bin/arm-linux-g++
BR_INCLUDE=$BUILDROOT_DIR/output/staging/usr/include
BR_LIB=$BUILDROOT_DIR/output/staging/usr/lib
BUILD_DIR=$BUILDROOT_DIR/output/build/wpeframework-custom/Source

CORE_O_DIR=$BUILD_DIR/core/CMakeFiles/WPEFrameworkCore.dir
TRACING_O_DIR=$BUILD_DIR/tracing/CMakeFiles/WPEFrameworkTracing.dir

# TODO: get from .pc file
CFLAGS='-std=c++11 -Wno-psabi -Wall  -D__NO_WCHAR_SUPPORT__ -DPLATFORM_PC_UNIX=1 -D_THUNDER_DEBUG'

rm -rf build
mkdir build
cd build

$GXX $CFLAGS -g -c -I$BR_INCLUDE/WPEFramework ../src/ResourceMonitorImplementation.cpp -o ResourceMonitorImplementation.o
#$GXX $CFLAGS -g -c -I$BR_INCLUDE/WPEFramework ../src/ResourceMonitorImplementation_ctor.cpp -o ResourceMonitorImplementation_ctor.o
$GXX $CFLAGS -g -c -I$BR_INCLUDE/WPEFramework ../src/main.cpp -o main.o

#$GXX ResourceMonitorImplementation.o ResourceMonitorImplementation_ctor.o main.o -o ResourceMonitor -L$BR_LIB -lWPEFrameworkCore -lWPEFrameworkTracing
#$GXX $CFLAGS -g $(ls $CORE_O_DIR/*.o $TRACING_O_DIR/*.o | grep -v Module.cpp.o) ResourceMonitorImplementation.o ResourceMonitorImplementation_ctor.o main.o -o ResourceMonitor -lpthread -lgcov -ldl
$GXX $CFLAGS -g $(ls $CORE_O_DIR/*.o $TRACING_O_DIR/*.o | grep -v Module.cpp.o) ResourceMonitorImplementation.o main.o -o ResourceMonitor -lpthread -lgcov -ldl

