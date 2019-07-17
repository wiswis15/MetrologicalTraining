#!/bin/bash

export PACKAGE_NAME=wpeframework
export EXTRA_CONF_ARGS='-DBUILD_TYPE=Debug -DJSONRPC=ON'
export SRC_DIR_OVERRIDE='/home/sander/Projects/WPEFramework/WPEFramework/src'

./build-cmake.sh

