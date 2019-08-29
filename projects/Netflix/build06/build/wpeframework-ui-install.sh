#!/bin/bash

set -e

rm -rf staging/usr/share/WPEFramework/Controller/UI
mkdir -p staging/usr/share/WPEFramework/Controller/UI

# ef53dcfa8d9dd4bf0894caf1367ab6fe2c2a48ee
cp -r repos/ThunderUI/build/* staging/usr/share/WPEFramework/Controller/UI

