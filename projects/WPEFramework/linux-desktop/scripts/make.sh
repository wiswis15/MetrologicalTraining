#!/bin/bash

make -C build.wpeframework/ install && make -C build.plugins.open/ install && make -C build.plugins.outofprocesstest/ install

