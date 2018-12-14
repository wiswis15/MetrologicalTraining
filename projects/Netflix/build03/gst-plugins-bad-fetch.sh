#!/bin/bash

mkdir -p zips
cd zips
rm -f gst-plugins-bad.tar.xz
wget -O gst-plugins-bad.tar.xz https://gstreamer.freedesktop.org/src/gst-plugins-bad/gst-plugins-bad-1.14.2.tar.xz

