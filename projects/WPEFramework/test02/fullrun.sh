#!/bin/bash

rm -rf build staging

# Fetch wpeframework and google test.
./wpeframework-fetch.sh
./gtest-fetch.sh

# Unzip and build google test.
./gtest-unzip.sh
./gtest-build.sh

# Unzip and build wpeframework.
./wpeframework-unzip.sh
./wpeframework-build.sh

# Run test runner for core part of wpeframework.
build/wpeframework/build/Tests/core/WPEFramework_test_core

# Generate coverage statistics for core part of wpeframework.
./generate-gcov.sh

