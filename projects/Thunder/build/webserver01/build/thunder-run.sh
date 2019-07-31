#!/bin/bash

# How to start in gdb:
#   THUNDER_PREFIX='gdb --args' ./thunder-run.sh

STAGING="$PWD/staging"

PATH=$PATH:$STAGING/usr/bin LD_LIBRARY_PATH=$STAGING/usr/lib $THUNDER_PREFIX $STAGING/usr/bin/WPEFramework -c $STAGING/etc/WPEFramework/config.json

