#!/bin/bash

rm -rf build staging

./wpeframework-copy.sh
./wpeframework-build.sh

./wpeframework-plugins-copy.sh
./wpeframework-plugins-patch.sh
./wpeframework-plugins-build.sh

./wpeframework-ocdm-copy.sh
./wpeframework-ocdm-build.sh

./ocdm-null2-copy.sh
./ocdm-null2-build.sh

./ocdmtestapp-copy.sh
./ocdmtestapp-build.sh

python/updateConfig.py

