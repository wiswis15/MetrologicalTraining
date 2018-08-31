#!/bin/bash

export PACKAGE_NAME=netflix5
export SRC_DIR_OVERRIDE=../src/netflix
export EXTRA_CONF_ARGS='-DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32 -DNRDP_HAS_AUDIOMIXER=OFF -DDPI_IMPLEMENTATION=reference -DDPI_REFERENCE_DRM=playready2.5-ss-tee'   

export TOOLCHAIN_DIRECTORY="$PWD/staging/usr"
export LD=ld

./build-cmake.sh

cd build/netflix5/build/src/platform/gibbon/data/dpi
rm -rf playready
ln -s ../../../../../staging/etc/playready

