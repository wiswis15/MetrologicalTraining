#!/bin/bash

set -e

rm -rf build staging

BASE_DIR=$PWD

PREFIX="$PWD/staging/usr"
mkdir -p $PREFIX

./openssl-unzip.sh
./openssl-build.sh

./gtest-unzip.sh
./gtest-build.sh

./thunder-complete-build.sh

