#!/bin/bash

cd build/icu/src
patch -p1 < ../../../patches/icu-use-locale.patch


