#!/bin/bash

mkdir -p build
cd build
rm -rf wpeframework
mkdir wpeframework
cd wpeframework
cp -r ../../../WPEFramework src

