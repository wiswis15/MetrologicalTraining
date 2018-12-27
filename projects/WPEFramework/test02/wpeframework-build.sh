#!/bin/bash

export PACKAGE_NAME=wpeframework
export EXTRA_CONF_ARGS='-DEXTERN_EVENTS=Decryption -DWPEFRAMEWORK_TEST_MEASUREMENTS=ON -DBUILD_TESTS=ON -DDISABLE_TRACING=ON'
export SRC_DIR_OVERRIDE='/home/sander/Projects/WPEFramework/eclipse/WPEFramework/src'

./build-cmake.sh

