#!/bin/bash

# Go into Core build dir.
cd build/wpeframework/build/Source/core/CMakeFiles/Core.dir

# Run gcov.
gcov *.gcno

