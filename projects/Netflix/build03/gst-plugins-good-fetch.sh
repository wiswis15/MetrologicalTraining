#!/bin/bash

mkdir -p zips
cd zips
rm -f gst-plugins-good.tar.xz
wget -O gst-plugins-good.tar.xz https://gstreamer.freedesktop.org/src/gst-plugins-good/gst-plugins-good-1.14.2.tar.xz

