#!/bin/bash

# How to start in gdb:
#   OCDMTESTAPP05_PREFIX='gdb --args' ./ocdmtestapp05-run.sh

STAGING="$PWD/staging"

export PATH=$PATH:$STAGING/usr/bin
export LD_LIBRARY_PATH=$STAGING/usr/lib

$OCDMTESTAPP05_PREFIX $STAGING/usr/bin/ocdmtestapp05

