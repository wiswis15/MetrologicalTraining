#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework-plugins wpeframework-plugins.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEFrameworkPlugins-legacy.git wpeframework-plugins
cd wpeframework-plugins
git checkout c20c70c84209839d6d9c2cbe12da34b89e509eef
rm -rf .git
cd ..
tar czf wpeframework-plugins.tgz wpeframework-plugins/
rm -rf wpeframework-plugins

