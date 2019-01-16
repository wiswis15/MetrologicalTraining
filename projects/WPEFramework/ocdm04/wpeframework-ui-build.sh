#!/bin/bash

STAGING_DIR=$PWD/staging/usr

rm -rf $STAGING_DIR/share/WPEFramework/Controller/UI
mkdir -p $STAGING_DIR/share/WPEFramework/Controller/UI
cp -vr build/wpeframework-ui/src/build/* $STAGING_DIR/share/WPEFramework/Controller/UI

