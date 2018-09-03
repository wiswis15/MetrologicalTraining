#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework-plugins wpeframework-plugins.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFrameworkPlugins.git wpeframework-plugins
cd wpeframework-plugins
git checkout d9f8ade99517d18071feb9016d4b516e8675e613
rm -rf .git
cd ..
tar czf wpeframework-plugins.tgz wpeframework-plugins/
rm -rf wpeframework-plugins

