#!/bin/bash

mkdir -p build/$PACKAGE_NAME
rm -rf build/$PACKAGE_NAME/src

mkdir -p build/$PACKAGE_NAME/src
cd build/$PACKAGE_NAME/src
tar xf ../../../zips/$ARCHIVE_NAME --strip 1

