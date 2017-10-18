#!/bin/bash

PACKAGE_NAME=wpebackend-rdk
BUILD_DIR=/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard
YOCTO_DIR=$BUILD_DIR/tmp/work/cortexa15hf-neon-vfpv4-rdk-linux-gnueabi/$PACKAGE_NAME/0.1-r0/git/
SOURCE_DIR=/home/sander/Projects/wpe/WPEBackend-rdk/eclipse/src

cd $BUILD_DIR
bitbake -f -c clean $PACKAGE_NAME && bitbake -f -c patch $PACKAGE_NAME && rm -rf $YOCTO_DIR/* && cp -r $SOURCE_DIR/* $YOCTO_DIR && bitbake $PACKAGE_NAME

