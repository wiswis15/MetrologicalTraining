#!/bin/bash

# How to start in gdb:
#   OCDMTESTAPP04_PREFIX='gdb --args' ./ocdmtestapp04-run.sh

STAGING="$PWD/staging"

export PATH=$PATH:$STAGING/usr/bin
export LD_LIBRARY_PATH=$STAGING/usr/lib

$OCDMTESTAPP04_PREFIX $STAGING/usr/bin/ocdmtestapp04

