#!/bin/bash

mkdir -p zips
cd zips
rm -f netflix5 netflix5.tgz
git clone git@github.com:Metrological/netflix netflix5
cd netflix5
git checkout 461a35e4885ca366d6e0b743a63ce1378e99f8ba
rm -rf .git
cd ..
tar czf netflix5.tgz netflix5
rm -rf netflix5

