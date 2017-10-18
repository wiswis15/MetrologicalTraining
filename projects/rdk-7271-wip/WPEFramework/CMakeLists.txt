cmake_minimum_required(VERSION 3.4)

project(WPEFramework)

set(WPEFRAMEWORK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(WPEFRAMEWORK_VERSION_MAJOR 1)
set(WPEFRAMEWORK_VERSION_MINOR 0)
set(WPEFRAMEWORK_VERSION "${WPEFRAMEWORK_VERSION_MAJOR}.${WPEFRAMEWORK_VERSION_MINOR}")

if (BUILD_REFERENCE)
    add_definitions (-DBUILD_REFERENCE=${BUILD_REFERENCE})
endif()

list(APPEND CMAKE_MODULE_PATH "${WPEFRAMEWORK_ROOT}/Source/cmake")

# Skip all rpaths for both build tree and target
set(CMAKE_SKIP_RPATH true)

add_subdirectory(Source)

if(WPEFRAMEWORK_TEST_APPS)
    add_subdirectory(tests)
endif()

# UI
# TODO: move UI out of this repo
file(GLOB UI UI/*) 
install(FILES ${UI} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/WPEFramework/Controller/UI)


