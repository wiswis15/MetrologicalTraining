#!/bin/bash

rm -rf build staging

./wpeframework-copy.sh
./wpeframework-build.sh

./wpeframework-ocdm-copy.sh
./wpeframework-ocdm-build.sh

./wpeframework-plugins-copy.sh
./wpeframework-plugins-patch.sh
./wpeframework-plugins-build.sh

./ocdm-null2-copy.sh
./ocdm-null2-build.sh

./ocdmtestapp01-copy.sh
./ocdmtestapp01-build.sh

./ocdmtestapp02-copy.sh
./ocdmtestapp02-build.sh

python/updateConfig.py

