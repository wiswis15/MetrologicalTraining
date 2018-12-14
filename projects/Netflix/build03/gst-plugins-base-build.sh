#!/bin/bash

export PACKAGE_NAME=gst-plugins-base
export EXTRA_CONF_ARGS='--disable-gl --disable-egl --disable-glx --disable-opengl --disable-xvideo --disable-ivorbis --disable-vorbis --disable-ogg --disable-examples'

./build-configure.sh

