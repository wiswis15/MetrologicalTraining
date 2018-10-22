#!/bin/bash

export SRC_DIR_OVERRIDE=/home/sander/Projects/netflix/5/netflix.ocdm/netflix

export PACKAGE_NAME=netflix5
export EXTRA_CONF_ARGS='-DNRDP_HAS_AUDIOMIXER=OFF -DDPI_IMPLEMENTATION=reference -DDPI_REFERENCE_DRM=ocdm -DNETFLIX5_SEARCH_INSTALL_PREFIX=ON'
export AFTER_MAKE_INSTALL_CMD='../../../patches/netflix5-setup-playready-symlink.sh'

export TOOLCHAIN_DIRECTORY="$PWD/staging/usr"
export LD=ld

./build-cmake.sh

