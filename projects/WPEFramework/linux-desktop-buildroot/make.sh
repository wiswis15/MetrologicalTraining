#!/bin/bash

make -j4 -C build/wpeframework/build install && make -j4 -C build/wpeframework-plugins/build install && make -j4 -C build/out-of-process/build install && python/updateConfig.py

