#!/bin/bash

SOURCE_DIR=/home/sander/Projects/buildroot/nos/ocdm-repos

rm -rf DrmExtensions
cp -r $SOURCE_DIR/DrmExtensions .

rm -rf OCDM-Null2
cp -r $SOURCE_DIR/OCDM-Null2 .

rm -rf ocdmtestapp01
cp -r $SOURCE_DIR/ocdmtestapp01 .

rm -rf ocdmtestapp01/build

rm -rf ocdmtestapp02
cp -r $SOURCE_DIR/ocdmtestapp02 .

rm -rf WPEFramework
cp -r $SOURCE_DIR/WPEFramework .

rm -rf WPEFramework/doc
rm -rf WPEFramework/.git

rm -rf WPEFrameworkOCDM
cp -r $SOURCE_DIR/WPEFrameworkOCDM .

rm -rf WPEFrameworkPlugins
cp -r $SOURCE_DIR/WPEFrameworkPlugins .

rm -rf WPEFrameworkPlugins/.git

rm -rf WPEFrameworkPlugins/Monitor/doc
rm -rf WPEFrameworkPlugins/RemoteControl/doc
rm -rf WPEFrameworkPlugins/Snapshot/doc
rm -rf WPEFrameworkPlugins/TraceControl/doc
rm -rf WPEFrameworkPlugins/WifiControl/doc

rm -f 'WPEFrameworkPlugins/WebKitBrowser/doc/WPE - API - WebKitBrowser.docx'

mkdir -p build01
rm -f build01/*.sh

cp $SOURCE_DIR/build01/*.sh build01


