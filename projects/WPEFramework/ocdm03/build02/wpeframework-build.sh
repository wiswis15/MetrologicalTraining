#!/bin/bash

export PACKAGE_NAME=wpeframework
export EXTRA_CONF_ARGS='-DWPEFRAMEWORK_CDMI=ON -DEXTERN_EVENTS=Decryption -DWPEFRAMEWORK_SYSTEM_PREFIX=NOS -DWPEFRAMEWORK_PROVISIONPROXY=ON'
export SRC_DIR_OVERRIDE='/home/sander/Projects/buildroot/nos/ocdm-repos/WPEFramework-netflix'

./build-cmake.sh

