#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework-ui wpeframework-ui.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFrameworkUI.git wpeframework-ui
cd wpeframework-ui
git checkout 7ff4eff4c66d24845efe0ba0bcb2fbc15a61d120
rm -rf .git
cd ..
tar czf wpeframework-ui.tgz wpeframework-ui/
rm -rf wpeframework-ui

