#!/bin/bash

rm -rf build staging staging.gtest

./gtest-unzip.sh
./gtest-build.sh

cp -r staging staging.gtest

./fullrun-framework-only.sh

