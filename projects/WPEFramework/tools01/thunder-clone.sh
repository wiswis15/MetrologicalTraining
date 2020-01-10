#!/bin/bash

set -e

mkdir -p repos
cd repos

rm -rf Thunder
git clone git@github.com:WebPlatformForEmbedded/Thunder.git Thunder

# TODO: once this branch is merged into master, no need to checkout commmit
cd Thunder
git checkout 8fe7c23aebed59a06c5a718ac1d1214d05ed6dfd


