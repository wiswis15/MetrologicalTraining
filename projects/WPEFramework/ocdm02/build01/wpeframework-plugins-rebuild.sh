#!/bin/bash

# TODO: diff followed by patch, by ./wpeframework-plugins-patch, followed by make

./wpeframework-plugins-copy.sh
./wpeframework-plugins-patch.sh
./wpeframework-plugins-build.sh

python/updateConfig.py

