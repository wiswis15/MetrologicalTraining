#!/bin/bash

set -e

cd build/wpeframework/build/Source/core/CMakeFiles/WPEFrameworkCore.dir
gcov *.gcno

