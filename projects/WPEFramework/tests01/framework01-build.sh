#!/bin/bash

export PACKAGE_NAME=framework01
export EXTRA_CONF_ARGS='-DFRAMEWORK01_BUILD_TESTS=ON -DFRAMEWORK01_COVERAGE=ON'

./build-cmake.sh

