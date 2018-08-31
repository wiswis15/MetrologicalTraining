#!/bin/bash

export PACKAGE_NAME=libprovision
export EXTRA_CONF_ARGS='-DCMAKE_SYSTEM_PROCESSOR=x86'

./build-cmake.sh

