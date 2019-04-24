#!/bin/bash

#export libprovision_DIR="$PWD/staging/usr/include/cmake/"

export PACKAGE_NAME=wpeframework
#export EXTRA_CONF_ARGS="-DCDMI=ON -DEXTERN_EVENTS=Decryption -DSYSTEM_PREFIX=NOS -DPROVISIONPROXY=ON -DBUILD_TYPE=Debug -Dlibprovision_DIR=$PWD/staging/usr/include/cmake/"
export EXTRA_CONF_ARGS="-DCDMI=ON -DEXTERN_EVENTS=Decryption -DSYSTEM_PREFIX=NOS -DPROVISIONPROXY=ON -DBUILD_TYPE=Debug"
export SRC_DIR_OVERRIDE='/home/sander/Projects/WPEFramework/build/new-cmake/repos/WPEFramework'

./build-cmake.sh

