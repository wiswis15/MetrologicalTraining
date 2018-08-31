#!/bin/bash

cd build/wpeframework/src
patch -p1 < ../../../patches/wpeframework-no-git-magic.patch

