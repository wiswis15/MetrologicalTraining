#!/bin/bash

rm -rf zips
mkdir -p zips
cd zips

wget https://raw.githubusercontent.com/WebPlatformForEmbedded/buildroot/master/package/wpe/wpeframework/wpeframework.mk
GIT_HASH=$(grep -oE '[0-9a-f]{40}' wpeframework.mk | head -n1)
wget -O wpeframework.tgz https://codeload.github.com/WebPlatformForEmbedded/WPEFramework/tar.gz/$GIT_HASH

wget https://raw.githubusercontent.com/WebPlatformForEmbedded/buildroot/master/package/wpe/wpeframework-plugins/wpeframework-plugins.mk
GIT_HASH=$(grep -oE '[0-9a-f]{40}' wpeframework-plugins.mk | head -n1)
wget -O wpeframework-plugins.tgz https://codeload.github.com/WebPlatformForEmbedded/WPEFrameworkPlugins/tar.gz/$GIT_HASH


