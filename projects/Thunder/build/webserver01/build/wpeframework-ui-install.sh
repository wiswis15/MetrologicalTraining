#!/bin/bash

set -e

rm -rf staging/usr/share/WPEFramework/Controller/UI
mkdir -p staging/usr/share/WPEFramework/Controller/UI

#cp -r repos/WPEFrameworkUI/src/* staging/usr/share/WPEFramework/Controller/UI
cp -r repos/WPEFrameworkUI/build/* staging/usr/share/WPEFramework/Controller/UI


