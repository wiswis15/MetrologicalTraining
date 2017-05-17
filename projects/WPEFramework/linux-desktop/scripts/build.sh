#!/bin/bash

BASEDIR=$PWD

rm -rf build.wpeframework/ build.plugins.open/ build.plugins.outofprocesstest/ prefix/
mkdir -p staging/usr

mkdir build.wpeframework
cd build.wpeframework
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$BASEDIR/staging/usr -DINSTALL_HEADERS_TO_TARGET=ON -DWPEFRAMEWORK_TEST_APPS=ON -DWPEFRAMEWORK_TEST_LOADER=ON ../eclipse/src/
make install
cd ..

mkdir build.plugins.open
cd build.plugins.open
PKG_CONFIG_PATH=$BASEDIR/staging/usr/lib/pkgconfig/ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$BASEDIR/staging/usr \
-DWPEFRAMEWORK_PLUGIN_COMMANDER=ON \
-DWPEFRAMEWORK_PLUGIN_DEVICEINFO=ON \
-DWPEFRAMEWORK_PLUGIN_LOCATIONSYNC=ON \
-DWPEFRAMEWORK_PLUGIN_MONITOR=ON \
-DWPEFRAMEWORK_PLUGIN_REMOTECONTROL=ON \
-DWPEFRAMEWORK_PLUGIN_TIMESYNC=ON \
-DWPEFRAMEWORK_PLUGIN_TRACECONTROL=ON \
-DWPEFRAMEWORK_PLUGIN_WEBPROXY=ON \
-DWPEFRAMEWORK_PLUGIN_WEBSERVER=ON \
-DWPEFRAMEWORK_PLUGIN_WEBSHELL=ON ../plugins/open/
make install
cd ..

mkdir build.plugins.outofprocesstest
cd build.plugins.outofprocesstest
PKG_CONFIG_PATH=$BASEDIR/staging/usr/lib/pkgconfig/ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$BASEDIR/staging/usr ../plugins/OutOfProcessTest/
make install
cd ..


