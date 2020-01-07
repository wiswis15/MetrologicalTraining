#!/bin/bash

export PACKAGE_NAME=thundernanoservices
export EXTRA_CONF_ARGS='-DBUILD_TYPE=Debug -DCMAKE_BUILD_TYPE=Debug -DPLUGIN_TRACECONTROL=ON -DPLUGIN_RESOURCEMONITOR=ON -DPLUGIN_REMOTECONTROL=ON'
export SRC_DIR_OVERRIDE="$PWD/repos/ThunderNanoServices"

./build-cmake.sh

