#!/bin/bash

set -e

rm -rf staging/usr/share/WPEFramework/Controller/UI
mkdir -p staging/usr/share/WPEFramework/Controller/UI

echo 'Copying Thunder UI'
cp -rv repos/ThunderUI/dist/* staging/usr/share/WPEFramework/Controller/UI


