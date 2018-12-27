cmake_minimum_required(VERSION 3.5)

option(FRAMEWORK01_BUILD_TESTS "Build tests (requires gtest)"       OFF)
option(FRAMEWORK01_COVERAGE    "Include code coverage measurments." OFF)

if(FRAMEWORK01_COVERAGE)
    list(APPEND CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS -lgcov)
endif(FRAMEWORK01_COVERAGE)

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(Source)

if(FRAMEWORK01_BUILD_TESTS)
    add_subdirectory(Tests)
endif(FRAMEWORK01_BUILD_TESTS)
