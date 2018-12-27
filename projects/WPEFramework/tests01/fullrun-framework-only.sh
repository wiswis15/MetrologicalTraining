#!/bin/bash

BASE_DIR=$PWD

rm -rf staging
cp -r staging.gtest staging

./framework01-unzip.sh
./framework01-build.sh

cd build/framework01/build/Tests/core/
./framework01_test_core

cd $BASE_DIR

# run gcov
cd build/framework01/build/Source/core/CMakeFiles/FrameworkCore.dir
echo 'Running gcov:'
gcov *.gcno

