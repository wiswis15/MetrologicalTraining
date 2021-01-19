#!/bin/bash

set -e

mkdir -p zips
cd zips

rm -f cmake.tgz
wget -O cmake.tgz https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4.tar.gz
