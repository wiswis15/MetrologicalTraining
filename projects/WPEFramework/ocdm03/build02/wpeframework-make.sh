#!/bin/bash

BASE_DIR=$PWD

cd build/wpeframework/build
make -j4 install

cd $BASE_DIR

python/updateConfig.py

