#!/bin/bash

set -e

rm -rf staging/usr/share/WPEFramework/Controller/UI
mkdir -p staging/usr/share/WPEFramework/Controller/UI

#cp -r repos/ThunderUI/src/* staging/usr/share/WPEFramework/Controller/UI
#cp -r repos/ThunderUI/build/* staging/usr/share/WPEFramework/Controller/UI
cp -r repos/ThunderUI/dist/* staging/usr/share/WPEFramework/Controller/UI


