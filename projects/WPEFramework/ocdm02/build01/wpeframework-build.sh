#!/bin/bash

export PACKAGE_NAME=wpeframework
export EXTRA_CONF_ARGS='-DWPEFRAMEWORK_CDMI=ON -DEXTERN_EVENTS=Decryption'

./build-cmake.sh

