#!/bin/bash

#BOUND_KEY_MESSAGE=$HOME/Projects/netflix/5/ESN/boundKeyMsg.bin
BOUND_KEY_MESSAGE=/tmp/boundKeyMsg.bin

export LD_LIBRARY_PATH=$PWD/staging/usr/lib

cd build/netflix5/build/src/platform/gibbon

cp $BOUND_KEY_MESSAGE .

./netflix -I boundKeyMsg.bin



