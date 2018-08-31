#!/bin/bash

export PACKAGE_NAME=wpeframework-plugins
export EXTRA_CONF_ARGS='-DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32 -DWPEFRAMEWORK_WEB_UI=ON -DWPEFRAMEWORK_PLUGIN_COMMANDER=ON -DWPEFRAMEWORK_PLUGIN_DEVICEINFO=ON -DWPEFRAMEWORK_PLUGIN_LOCATIONSYNC=ON -DWPEFRAMEWORK_PLUGIN_MONITOR=ON -DWPEFRAMEWORK_PLUGIN_TIMESYNC=ON -DWPEFRAMEWORK_PLUGIN_TRACECONTROL=ON -DWPEFRAMEWORK_PLUGIN_WEBPROXY=ON -DWPEFRAMEWORK_PLUGIN_LOCATIONSYNC_URI=http://jsonip.metrological.com/?maf=true'

# TODO: WebServer seems to crash during start-up
# -DWPEFRAMEWORK_PLUGIN_WEBSERVER=ON

# Better to not build webshell, allows for execution of commands
# -DWPEFRAMEWORK_PLUGIN_WEBSHELL=ON

# Compile error when building remote control
# -DWPEFRAMEWORK_PLUGIN_REMOTECONTROL=ON

./build-cmake.sh

