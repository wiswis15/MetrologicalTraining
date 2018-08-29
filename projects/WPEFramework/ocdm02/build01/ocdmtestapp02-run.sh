#!/bin/bash

# How to start in gdb:
#   OCDMTESTAPP02_PREFIX='gdb --args' ./ocdmtestapp02-run.sh

STAGING="$PWD/staging"

export PATH=$PATH:$STAGING/usr/bin
export LD_LIBRARY_PATH=$STAGING/usr/lib

$OCDMTESTAPP02_PREFIX $STAGING/usr/bin/ocdmtestapp02

