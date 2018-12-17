#!/bin/bash

export SRC_DIR_OVERRIDE=/home/sander/Projects/netflix/5/netflix/netflix

export PACKAGE_NAME=netflix5
export EXTRA_CONF_ARGS='-DNRDP_HAS_AUDIOMIXER=OFF -DDPI_IMPLEMENTATION=sink-interface -DDPI_SINK_INTERFACE_IMPLEMENTATION=null -DDPI_REFERENCE_DRM=playready2.5-ss-tee-install-prefix -DNETFLIX5_SEARCH_INSTALL_PREFIX=ON -DNETFLIX_USE_PROVISION=OFF -DDPI_REFERENCE_TTS=none -DBUILD_SIGNUP=OFF -DDPI_SINK_INTERFACE_OVERRIDE_APPBOOT=OFF'
export AFTER_MAKE_INSTALL_CMD='../../../patches/netflix5-setup-playready-symlink.sh'

export TOOLCHAIN_DIRECTORY="$PWD/staging/usr"
export LD=ld

./build-cmake.sh

