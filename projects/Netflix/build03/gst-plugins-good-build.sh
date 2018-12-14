#!/bin/bash

export PACKAGE_NAME=gst-plugins-good
export EXTRA_CONF_ARGS='--disable-gst_v4l2 --disable-flac --disable-speex --disable-soup --disable-vpx --disable-wavpack --disable-examples --disable-pulse --disable-libpng --disable-taglib --disable-pulse'

./build-configure.sh

