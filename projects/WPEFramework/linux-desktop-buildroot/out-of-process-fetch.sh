#!/bin/bash

mkdir -p zips
cd zips

ZIP_DIR=$PWD

rm -rf out-of-process out-of-process.tgz

git clone git@github.com:Metrological/Documentation.git out-of-process
cd out-of-process

git checkout sander

cd projects/WPEFramework/plugins/

tar czf out-of-process.tgz OutOfProcessTest

TAR_DIR=$PWD

cd $ZIP_DIR

mv $TAR_DIR/out-of-process.tgz .

rm -rf out-of-process

