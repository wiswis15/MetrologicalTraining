#!/bin/bash

set -e

make -j4 -C build/thunder/build install
make -j4 -C build/thunder-nano-services/build install
python/updateConfig.py

