#!/bin/bash

mkdir -p zips
cd zips
rm -f libprovision libprovision.tgz
git clone git@github.com:Metrological/libprovision.git
cd libprovision
git checkout 7e12a81534f797ca62b7590c0572a5ed75e459fb
rm -rf .git
cd ..
tar czf libprovision.tgz libprovision/
rm -rf libprovision

