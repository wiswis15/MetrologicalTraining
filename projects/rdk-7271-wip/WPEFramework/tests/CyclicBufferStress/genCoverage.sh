#!/bin/bash

rm -rf coverages
mkdir coverages

cd coverages
gcov ../build/Source/core/CMakeFiles/WPEFrameworkCore.dir/CyclicBuffer.cpp.gcno

echo "The coverage files are in the 'coverages/' dir, of special interest are CyclicBuffer.cpp.gcov and CyclicBuffer.h.gcov."

