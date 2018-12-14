#!/bin/bash

mkdir -p zips
cd zips
rm -f gst-plugins-libav.tar.xz
wget -O gst-plugins-libav.tar.xz https://gstreamer.freedesktop.org/src/gst-libav/gst-libav-1.14.2.tar.xz

