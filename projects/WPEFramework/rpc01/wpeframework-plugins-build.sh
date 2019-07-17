#!/bin/bash

export PACKAGE_NAME=wpeframework-plugins

# Gives crash:
# -DPLUGIN_REMOTECONTROL=ON
export EXTRA_CONF_ARGS='-DBUILD_TYPE=Debug -DPLUGIN_DEVICEINFO=ON -DPLUGIN_LOCATIONSYNC=ON -DPLUGIN_TRACECONTROL=ON -DPLUGIN_LOCATIONSYNC_URI=http://jsonip.metrological.com/?maf=true -DPLUGIN_JSONRPC=ON'
export SRC_DIR_OVERRIDE='/home/sander/Projects/WPEFramework/WPEFrameworkPlugins/src'

./build-cmake.sh

