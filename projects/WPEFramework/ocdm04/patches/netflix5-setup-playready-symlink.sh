#!/bin/bash

BUILD_DIR=$PWD

cd src/platform/gibbon/data/dpi
rm -rf playready
ln -s ../../../../../staging/etc/playready

cd $BUILD_DIR

cp src/platform/gibbon/netflix ../../../staging/usr/bin

