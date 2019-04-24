#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework-ui wpeframework-ui.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFrameworkUI.git wpeframework-ui
cd wpeframework-ui
git checkout e89325e5ea0575e6256dde462020ba06158d7abb
rm -rf .git
cd ..
tar czf wpeframework-ui.tgz wpeframework-ui/
rm -rf wpeframework-ui

