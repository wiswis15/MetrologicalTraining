#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework-provisioning wpeframework-provisioning.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEPluginProvisioning.git wpeframework-provisioning
cd wpeframework-provisioning
git checkout 105a9444e2ebbee6ae82e61af858242c97a0c4d3
rm -rf .git
cd ..
tar czf wpeframework-provisioning.tgz wpeframework-provisioning/
rm -rf wpeframework-provisioning

