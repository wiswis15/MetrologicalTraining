#!/bin/bash

export PACKAGE_NAME=gst-plugins-bad
export EXTRA_CONF_ARGS='--disable-yadif --disable-neon --disable-openal --disable-sndfile --disable-hls --disable-curl --disable-webp'

./build-configure.sh

