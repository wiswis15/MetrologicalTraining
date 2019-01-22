#!/bin/bash

mkdir -p zips
cd zips
rm -f libprovision libprovision.tgz
git clone git@github.com:Metrological/libprovision.git
cd libprovision
git checkout 5ace4f1c9646afe6621055b5ab2a4936e2c18f42
rm -rf .git
cd ..
tar czf libprovision.tgz libprovision/
rm -rf libprovision

