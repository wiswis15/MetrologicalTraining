#!/bin/bash

set -e

BASE_DIR=$PWD

cd ../../staging/usr/lib

export LD_LIBRARY_PATH=$PWD

cd $BASE_DIR

build/encrypt ../04-create_ESN/output/idfile out.bin

