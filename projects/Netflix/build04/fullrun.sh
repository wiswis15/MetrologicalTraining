#!/bin/bash

rm -rf build staging

./libprovision-unzip.sh
./libprovision-patch.sh
./libprovision-build.sh

./wpeframework-build.sh
./wpeframework-plugins-build.sh
./wpeframework-provisioning-build.sh

./playready-unzip.sh
./playready-patch.sh
./playready-build.sh

./ocdm-playready-build.sh
./ocdm-null2-build.sh
./ocdmtestapp03-build.sh

./netflix5-deps-unzip.sh
./netflix5-deps-build.sh

./netflix5-unzip.sh
./netflix5-build.sh

python/updateConfig.py

