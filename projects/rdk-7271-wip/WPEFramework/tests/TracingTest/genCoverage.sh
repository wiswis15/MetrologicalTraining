#!/bin/bash

rm -rf coverages
mkdir coverages

cd coverages
gcov ../build/Source/tracing/CMakeFiles/WPEFrameworkTracing.dir/TraceUnit.cpp.gcno
