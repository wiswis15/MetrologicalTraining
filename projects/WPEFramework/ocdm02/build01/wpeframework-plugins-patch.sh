#!/bin/bash

cd build/wpeframework-plugins/src

patch -p1 < ../../../patches/wpeframework-plugins-build-lib-in-build-dir.patch
patch -p1 < ../../../patches/wpeframework-plugins-ocdm-find-lib.patch

