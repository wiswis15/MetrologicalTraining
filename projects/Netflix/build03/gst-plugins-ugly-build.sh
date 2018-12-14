#!/bin/bash

export PACKAGE_NAME=gst-plugins-ugly
export EXTRA_CONF_ARGS='--disable-x264'

./build-configure.sh

