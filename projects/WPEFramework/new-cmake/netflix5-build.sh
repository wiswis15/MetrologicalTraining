#!/bin/bash

export SRC_DIR_OVERRIDE=/home/sander/Projects/Netflix/5.0/netflix/netflix

export PACKAGE_NAME=netflix5
export EXTRA_CONF_ARGS='-DNRDP_HAS_AUDIOMIXER=OFF -DDPI_IMPLEMENTATION=reference -DDPI_REFERENCE_DRM=ocdm -DNETFLIX5_SEARCH_INSTALL_PREFIX=ON -DNETFLIX_USE_PROVISION=OFF -DDPI_REFERENCE_TTS=none -DBUILD_SIGNUP=OFF -DDPI_SINK_INTERFACE_OVERRIDE_APPBOOT=OFF -DNRDP_CRASH_REPORTING=off'
export AFTER_MAKE_INSTALL_CMD='../../../patches/netflix5-setup-playready-symlink.sh'

export TOOLCHAIN_DIRECTORY="$PWD/staging/usr"
export LD=ld

./build-cmake.sh

