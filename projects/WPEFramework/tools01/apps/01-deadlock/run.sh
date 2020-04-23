#!/bin/bash

set -e

BASE_DIR=$PWD

cd ../../staging/usr/lib

export LD_LIBRARY_PATH=$PWD

cd $BASE_DIR

cd build
./app01

