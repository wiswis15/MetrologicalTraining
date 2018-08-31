#!/bin/bash

export PACKAGE_NAME=wpeframework-provisioning
export EXTRA_CONF_ARGS='-DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32 -DWPEFRAMEWORK_PROVISIONING_URI=provisioning-sdk.metrological.com:80'

./build-cmake.sh

