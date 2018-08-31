#!/bin/bash

STAGING_DIR="$PWD/staging"
PERSISTENT_PATH="$STAGING_DIR/persistent-path"
DATA_PATH="$STAGING_DIR/usr/share/WPEFramework"
SYSTEM_PATH="$STAGING_DIR/usr/lib/wpeframework/plugins"
PROXYSTUB_PATH="$STAGING_DIR/usr/lib/wpeframework/proxystubs"

mkdir -p $PERSISTENT_PATH

export PACKAGE_NAME=wpeframework

export EXTRA_CONF_ARGS="-DCMAKE_BUILD_TYPE=Debug -DWPEFRAMEWORK_PROVISIONPROXY=ON -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32 -DWPEFRAMEWORK_VIRTUALINPUT=ON -DWPEFRAMEWORK_INTERFACES_LIB=ON -DWPEFRAMEWORK_PERSISTENT_PATH=$PERSISTENT_PATH -DWPEFRAMEWORK_DATA_PATH=$DATA_PATH -DWPEFRAMEWORK_SYSTEM_PATH=$SYSTEM_PATH -DWPEFRAMEWORK_PROXYSTUB_PATH=$PROXYSTUB_PATH -DWPEFRAMEWORK_PORT=8081"

./build-cmake.sh

