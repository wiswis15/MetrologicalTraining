#!/bin/bash

set -e

rm -rf repos
mkdir repos
cd repos

# Note that the checked-out hashes worked on January 19, 2021.
# Feel free to comment-out the "cd ...; git checkout ... ; cd .." commands, it might still work.

git clone git@github.com:rdkcentral/Thunder.git
cd Thunder
git checkout e00a49e8fb2963f3e968f4d407847da02c1b7684
cd ..

git clone git@github.com:rdkcentral/ThunderInterfaces.git
cd ThunderInterfaces
git checkout f63c19c5efa97b1a0c0ee64e4edce61e04d206cb

# TODO: this needs to be merged
patch -p1 < ../../patches/thunder-interfaces-fix-generators-include-dirs.patch

cd ..

git clone git@github.com:rdkcentral/ThunderNanoServices.git
cd ThunderNanoServices
git checkout 23bd5e97f9462187357edd96ce8103f1d7c8f113
cd ..

git clone git@github.com:rdkcentral/ThunderUI.git
cd ThunderUI
git checkout 588cee55597c4b74e2ead6bf6452b41a99f4837f
cd ..

