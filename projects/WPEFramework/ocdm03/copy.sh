#!/bin/bash

SOURCE_DIR=/home/sander/Projects/buildroot/nos/ocdm-repos

rm -rf OCDM-Null2
cp -r $SOURCE_DIR/OCDM-Null2 .

rm -rf ocdmtestapp03
cp -r $SOURCE_DIR/ocdmtestapp03 .

rm -rf build02
mkdir -p build02

cp $SOURCE_DIR/build02/*.sh build02
cp -r $SOURCE_DIR/build02/python build02
cp -r $SOURCE_DIR/build02/patches build02

cp -r $SOURCE_DIR/build02/apps build02

rm -rf build02/apps/02-rpc/build
rm -rf build02/apps/03-out_of_process/build
rm -rf build02/apps/04-plugin01/build
rm -rf build02/apps/04-plugin01/build_nos
rm -rf build02/apps/05-plugin02/build
rm -rf build02/apps/05-plugin02/build_nos

