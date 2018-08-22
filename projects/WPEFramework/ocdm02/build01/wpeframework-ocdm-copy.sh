#!/bin/bash

mkdir -p build
cd build
rm -rf wpeframework-ocdm
mkdir wpeframework-ocdm
cd wpeframework-ocdm
cp -r ../../../WPEFrameworkOCDM src

