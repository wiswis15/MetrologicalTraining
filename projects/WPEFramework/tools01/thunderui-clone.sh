#!/bin/bash

set -e

mkdir -p repos
cd repos

rm -rf ThunderUI
git clone git@github.com:WebPlatformForEmbedded/ThunderUI.git ThunderUI

