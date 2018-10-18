#!/bin/bash

rm -rf build staging

./wpeframework-build.sh
./wpeframework-plugins-build.sh
./ocdm-null2-build.sh
./ocdmtestapp03-build.sh

python/updateConfig.py

