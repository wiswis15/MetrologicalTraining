#!/bin/bash

export PACKAGE_NAME=wpeframework-plugins
export EXTRA_CONF_ARGS='-DWPEFRAMEWORK_WEB_UI=ON -DWPEFRAMEWORK_PLUGIN_COMMANDER=ON -DWPEFRAMEWORK_PLUGIN_DEVICEINFO=ON -DWPEFRAMEWORK_PLUGIN_LOCATIONSYNC=ON -DWPEFRAMEWORK_PLUGIN_MONITOR=ON -DWPEFRAMEWORK_PLUGIN_REMOTECONTROL=ON -DWPEFRAMEWORK_PLUGIN_TIMESYNC=ON -DWPEFRAMEWORK_PLUGIN_TRACECONTROL=ON -DWPEFRAMEWORK_PLUGIN_WEBPROXY=ON -DWPEFRAMEWORK_PLUGIN_WEBSERVER=ON -DWPEFRAMEWORK_PLUGIN_WEBSHELL=ON'

./build-cmake.sh