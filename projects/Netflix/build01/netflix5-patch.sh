#!/bin/bash

cd build/netflix5/src
patch -p1 < ../../../patches/netflix5-reference-cmake-prefix-paths.patch

