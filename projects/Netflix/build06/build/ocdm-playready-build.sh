#!/bin/bash

export PACKAGE_NAME=ocdm-playready
export SRC_DIR_OVERRIDE="$PWD/repos/OCDM-Playready"
export AFTER_MAKE_INSTALL_CMD='../../../patches/ocdm-playready-setup-dirs.sh'

./build-cmake.sh

