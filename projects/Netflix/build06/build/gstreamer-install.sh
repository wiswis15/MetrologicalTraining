#!/bin/bash

set -e

tar xf zips/gstreamer-32bits-staging.tgz

sed -i "s,/home/build,$PWD,g" staging/usr/lib/pkgconfig/*.pc

