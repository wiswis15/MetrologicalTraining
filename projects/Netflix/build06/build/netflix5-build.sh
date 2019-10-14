#!/bin/bash

export SRC_DIR_OVERRIDE=/home/sander/Projects/Netflix/5.0/build/repos/netflix/netflix

export PACKAGE_NAME=netflix5

export EXTRA_CONF_ARGS='-DNRDP_HAS_AUDIOMIXER=OFF -DDPI_IMPLEMENTATION=reference -DNETFLIX5_SEARCH_INSTALL_PREFIX=ON -DNETFLIX_USE_PROVISION=OFF -DDPI_REFERENCE_TTS=none -DBUILD_SIGNUP=OFF -DDPI_DRM=ocdm -DJS_MINIFY=OFF -DNRDP_HAS_TRACING=ON -DNRDP_TOOLS=provisioning'

export AFTER_MAKE_INSTALL_CMD='../../../patches/netflix5-setup-playready-symlink.sh'
export MAKE_NO_MAKE_INSTALL=y

export TOOLCHAIN_DIRECTORY="$PWD/staging/usr"
export LD=ld

./build-cmake.sh

