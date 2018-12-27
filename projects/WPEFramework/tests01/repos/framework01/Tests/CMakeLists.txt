find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Source)

enable_testing()

add_subdirectory(core)
