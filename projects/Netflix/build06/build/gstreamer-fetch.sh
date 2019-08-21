#!/bin/bash

mkdir -p zips
cd zips
rm -f gstreamer.tar.xz
wget -O gstreamer.tar.xz https://gstreamer.freedesktop.org/src/gstreamer/gstreamer-1.14.1.tar.xz

