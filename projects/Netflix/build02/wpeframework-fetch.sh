#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework wpeframework.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFramework-legacy.git wpeframework
cd wpeframework
git checkout 839e9ccabe2ac0dc272f1c2a41769da39534a3e8
rm -rf .git
cd ..
tar czf wpeframework.tgz wpeframework/
rm -rf wpeframework

