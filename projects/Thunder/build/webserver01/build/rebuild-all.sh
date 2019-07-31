#!/bin/bash

set -e

rm -rf build staging

./thunder-build.sh
./thunder-nano-services-build.sh
./wpeframework-ui-install.sh

python/updateConfig.py

