#!/bin/bash

mkdir -p build
cd build
rm -rf wpeframework-plugins
mkdir wpeframework-plugins
cd wpeframework-plugins
cp -r ../../../WPEFrameworkPlugins src

