#!/bin/bash

mkdir -p zips
cd zips

rm -f icu.tgz

# 58.3:
#wget -O icu.tgz https://www.github.com/unicode-org/icu/archive/9248183bc6ab67503d03d4a6b4c842e77e3fd1b8.tar.gz

# 58.2:
wget -O icu.tgz https://www.github.com/unicode-org/icu/archive/bc11f589a90d1881ed06578666a831ad7d072d18.tar.gz

