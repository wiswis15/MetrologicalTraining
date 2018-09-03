#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework wpeframework.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFramework.git wpeframework
cd wpeframework
git checkout dc64db6ab27e2ed9d80411da725e7b63df416956
rm -rf .git
cd ..
tar czf wpeframework.tgz wpeframework/
rm -rf wpeframework

