#!/bin/bash

export PACKAGE_NAME=wpeframework-plugins
export EXTRA_CONF_ARGS='-DPLUGIN_TRACECONTROL=ON -DPLUGIN_OPENCDMI=ON -DPLUGIN_OPENCDMI_NULL=ON -DPLUGIN_OPENCDMI_PLAYREADY=ON'
export SRC_DIR_OVERRIDE="$PWD/repos/ThunderNanoServices"

./build-cmake.sh

