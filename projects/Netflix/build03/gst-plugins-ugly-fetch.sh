#!/bin/bash

mkdir -p zips
cd zips
rm -f gst-plugins-ugly.tar.xz
wget -O gst-plugins-ugly.tar.xz https://gstreamer.freedesktop.org/src/gst-plugins-ugly/gst-plugins-ugly-1.14.2.tar.xz

