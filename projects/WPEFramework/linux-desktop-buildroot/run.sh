#!/bin/bash

# How to start in gdb:
#   WPEFRAMEWORK_PREFIX='gdb --args' ./run.sh

STAGING="$PWD/staging"

PATH=$PATH:$STAGING/usr/bin LD_LIBRARY_PATH=$STAGING/usr/lib $WPEFRAMEWORK_PREFIX $STAGING/usr/bin/WPEFramework -c $STAGING/etc/WPEFramework/config.json

