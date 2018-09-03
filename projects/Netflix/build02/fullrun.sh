#!/bin/bash

rm -rf build staging

mkdir build
mkdir -p staging/usr

./libprovision-unzip.sh
./libprovision-patch.sh
./libprovision-build.sh

./wpeframework-unzip.sh
./wpeframework-build.sh

./playready-unzip.sh
./playready-patch.sh
./playready-build.sh

./wpeframework-plugins-unzip.sh
./wpeframework-plugins-patch.sh
./wpeframework-plugins-build.sh

./wpeframework-ui-unzip.sh
./wpeframework-ui-build.sh

./wpeframework-provisioning-unzip.sh
./wpeframework-provisioning-build.sh

./copy-netflix5-deps.sh

./netflix5-unzip.sh
./netflix5-patch.sh
./netflix5-build.sh

python/updateConfig.py


