#!/bin/bash

export EXTRA_CONF_ARGS='--with-library-bits=32 --disable-layoutex'
export PACKAGE_NAME=icu
export NO_CONFIGURE_TOOLCHAIN_OVERRIDE=y
export CONF_OVERRIDE_LOC='../src/icu4c/source/configure'
export MAKE_NO_MAKE_INSTALL='y'
export AFTER_MAKE_INSTALL_CMD='../../../patches/icu-install-icu-data.sh'

export CXXFLAGS='--std=c++0x'

./build-configure.sh

