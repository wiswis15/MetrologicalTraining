#!/bin/bash

mkdir -p zips
cd zips
rm -f playready playready.tgz
git clone git@github.com:Metrological/playready.git
cd playready
git checkout 5af745dd2a8312a34081150c8b374684799fc1c9
rm -rf .git
cd ..
tar czf playready.tgz playready/
rm -rf playready


