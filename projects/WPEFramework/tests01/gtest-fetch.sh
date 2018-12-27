#!/bin/bash

mkdir -p zips
cd zips

rm -f gtest.tgz

# Hash: ec44c6c1675c25b9827aacd08c02433cccde7780
wget -O gtest.tgz https://github.com/google/googletest/archive/release-1.8.0.tar.gz

