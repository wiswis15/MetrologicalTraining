#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework-ui wpeframework-ui.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFrameworkUI-legacy.git wpeframework-ui
cd wpeframework-ui
git checkout d7b707d4031d501a2d4d33896dce7393762d601f
rm -rf .git
cd ..
tar czf wpeframework-ui.tgz wpeframework-ui/
rm -rf wpeframework-ui

