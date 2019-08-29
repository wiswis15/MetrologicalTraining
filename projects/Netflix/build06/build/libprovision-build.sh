#!/bin/bash

export PACKAGE_NAME=libprovision
export EXTRA_CONF_ARGS='-DARCH_OVERRIDE=x86'
export SRC_DIR_OVERRIDE="$PWD/repos/libprovision"

./build-cmake.sh

