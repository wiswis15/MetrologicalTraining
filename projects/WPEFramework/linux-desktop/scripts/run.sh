#!/bin/bash

# How to start in gdb:
#   WPEFRAMEWORK_PREFIX='gdb --args' ./run.sh

PATH=$PATH:/home/sander/Projects/WPEFramework/staging/usr/bin LD_LIBRARY_PATH=/home/sander/Projects/WPEFramework/staging/usr/lib $WPEFRAMEWORK_PREFIX /home/sander/Projects/WPEFramework/staging/usr/bin/WPEFramework -c /home/sander/Projects/WPEFramework/configuration/config.json

