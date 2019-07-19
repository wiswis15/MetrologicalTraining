#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework wpeframework.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFramework.git wpeframework
cd wpeframework
git checkout b7188636e6fc49b5cd92ee6efdb00afdeca993c4
rm -rf .git
cd ..
tar czf wpeframework.tgz wpeframework
rm -rf wpeframework

