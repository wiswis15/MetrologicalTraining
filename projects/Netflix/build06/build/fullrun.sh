#!/bin/bash

set -e

rm -rf build staging

./netflix5-deps-unzip.sh
./netflix5-deps-build.sh

./gstreamer-install.sh

./icu-unzip.sh
./icu-patch.sh
./icu-build.sh

./libprovision-build.sh

./wpeframework-build.sh
./wpeframework-plugins-build.sh
./wpeframework-provisioning-build.sh
./wpeframework-ui-install.sh

./playready-unzip.sh
./playready-patch.sh
./playready-build.sh

./ocdm-playready-build.sh

./ffmpeg-unzip.sh
./ffmpeg-build.sh

./netflix5-build.sh

python/updateConfig.py

