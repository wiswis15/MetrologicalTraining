#!/bin/bash

PACKAGE_NAME=WPEFramework
BUILD_DIR=/home/sander/Projects/yocto/rpi-yocto/rpi-ml-build
YOCTO_DIR=$BUILD_DIR/tmp/work/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/WPEFramework/1.0+gitrAUTOINC+f6f4feb0fa-r0/git
SOURCE_DIR=/home/sander/Projects/WPEFramework/eclipse/src

cd $BUILD_DIR
bitbake -f -c clean $PACKAGE_NAME && bitbake -f -c patch $PACKAGE_NAME && rm -rf $YOCTO_DIR/* && cp -r $SOURCE_DIR/* $YOCTO_DIR && bitbake $PACKAGE_NAME

