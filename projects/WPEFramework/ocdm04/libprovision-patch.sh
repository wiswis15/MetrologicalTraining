#!/bin/bash

cd build/libprovision/src
patch -p1 < ../../../patches/libprovision-force-32-bits.patch

