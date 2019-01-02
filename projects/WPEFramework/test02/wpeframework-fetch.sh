#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework wpeframework.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFramework.git wpeframework
cd wpeframework
git checkout 6a91c8e99d7fe16ef95255b5fb4e8aeff1654690
rm -rf .git
cd ..
tar czf wpeframework.tgz wpeframework
rm -rf wpeframework

