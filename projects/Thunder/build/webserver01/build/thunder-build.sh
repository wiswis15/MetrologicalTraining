#!/bin/bash

export PACKAGE_NAME=thunder
export EXTRA_CONF_ARGS='-DBUILD_TYPE=Debug'
export SRC_DIR_OVERRIDE='/home/sander/Projects/WPEFramework/build/webserver01/repos/Thunder'

./build-cmake.sh

