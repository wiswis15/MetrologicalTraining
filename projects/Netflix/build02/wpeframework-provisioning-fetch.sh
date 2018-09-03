#!/bin/bash

mkdir -p zips
cd zips
rm -f wpeframework-provisioning wpeframework-provisioning.tgz
git clone git@github.com:WebPlatformForEmbedded/WPEPluginProvisioning.git wpeframework-provisioning
cd wpeframework-provisioning
git checkout 43fac8ca8f208ef06ea1a0bda673318be17be5ff
rm -rf .git
cd ..
tar czf wpeframework-provisioning.tgz wpeframework-provisioning/
rm -rf wpeframework-provisioning

