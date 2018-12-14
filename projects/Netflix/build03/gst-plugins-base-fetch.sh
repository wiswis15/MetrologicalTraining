#!/bin/bash

mkdir -p zips
cd zips
rm -f gst-plugins-base.tar.xz
wget -O gst-plugins-base.tar.xz https://gstreamer.freedesktop.org/src/gst-plugins-base/gst-plugins-base-1.14.2.tar.xz

