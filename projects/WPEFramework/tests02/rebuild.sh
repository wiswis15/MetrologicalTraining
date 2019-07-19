#!/bin/bash

set -e

rm -rf build staging

mkdir -p staging/usr

./gtest-unzip.sh
./gtest-build.sh

./wpeframework-unzip.sh
./wpeframework-build.sh

