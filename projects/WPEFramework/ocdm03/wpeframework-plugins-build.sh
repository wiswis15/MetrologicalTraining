#!/bin/bash

export PACKAGE_NAME=wpeframework-plugins
export EXTRA_CONF_ARGS='-DWPEFRAMEWORK_PLUGIN_TRACECONTROL=ON -DWPEFRAMEWORK_PLUGIN_OPENCDMI=ON -DPLUGIN_OPENCDMI_NULL=ON'
export SRC_DIR_OVERRIDE='/home/sander/Projects/buildroot/nos/ocdm-repos/WPEFrameworkPlugins-netflix'

./build-cmake.sh

