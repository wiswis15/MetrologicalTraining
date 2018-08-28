#!/bin/bash

# How to start in gdb:
#   OCDMTESTAPP01_PREFIX='gdb --args' ./ocdmtestapp01-run.sh

STAGING="$PWD/staging"

export PATH=$PATH:$STAGING/usr/bin
export LD_LIBRARY_PATH=$STAGING/usr/lib

$OCDMTESTAPP01_PREFIX $STAGING/usr/bin/ocdmtestapp01

