#!/bin/bash

rm -rf build staging

./gtest-unzip.sh
./gtest-build.sh

./wpeframework-build.sh

build/wpeframework/build/Tests/core/WPEFramework_test_core
./generate-gcov.sh


