#!/bin/bash

set -e

rm -rf build staging

./cmake-build.sh
./thunder-tools-build.sh
./thunder-build.sh
./thunder-interfaces-build.sh
./thunder-nanoservices-build.sh
./thunder-ui-install.sh

