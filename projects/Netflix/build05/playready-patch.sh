#!/bin/bash

cd build/playready/src
patch -p1 < ../../../patches/playready-install-etc.patch
patch -p1 < ../../../patches/playready-make-lib-shared.patch

