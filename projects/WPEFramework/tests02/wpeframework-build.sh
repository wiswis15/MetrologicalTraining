#!/bin/bash

export PACKAGE_NAME=wpeframework
export EXTRA_CONF_ARGS='-DBUILD_TYPE=Debug -DBUILD_TESTS=ON -DWPEFRAMEWORK_TEST_MEASUREMENTS=ON -DDISABLE_TRACING=ON'
export SRC_DIR_OVERRIDE='/home/sander/Projects/WPEFramework/WPEFramework/src'

./build-cmake.sh

