#!/bin/bash

export PACKAGE_NAME=playready

export EXTRA_CONF_ARGS='-DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32'
export SRC_DIR_OVERRIDE='../src/src'

./build-cmake.sh

