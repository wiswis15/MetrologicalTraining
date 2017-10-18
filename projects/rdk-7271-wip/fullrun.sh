#!/bin/bash

rm -rf tmp
find /tmp/preload-log/ -type f | xargs rm
bitbake rdk-generic-wpe-westeros
cp -r /tmp/preload-log /home/sander/Personal/experiments/linux/execvPreload01/preload01/traces
cd /home/sander/Personal/experiments/linux/execvPreload01/preload01/traces
mv preload-log 09-real-full
find /tmp/preload-log/ -type f | xargs rm

