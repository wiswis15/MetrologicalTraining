cmake_minimum_required(VERSION 3.5)

project(wpeframework_unit_test)

include(setup_target_properties_executable)
include(show_target_properties)

message("Setting up wpeframework_unit_test")

find_package(GTEST REQUIRED)

set(WPEFRAMEWORK_UNIT_TESTS_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(WPEFRAMEWORK_UNIT_TESTS_INCLUDE
    support
    ${GTEST_INCLUDE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(WPEFRAMEWORK_UNIT_TESTS_LIBS
    ${GTEST_LIBRARIES}
    generics
    cryptalgo)

set(WPEFRAMEWORK_UNIT_TESTS_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${WPEFRAMEWORK_UNIT_TESTS_LIBS}
    CACHE INTERNAL "${PROJECT_NAME}: Link libraries" FORCE)

file(GLOB_RECURSE WPEFRAMEWORK_UNIT_TESTS_INCLUDES *.h)
file(GLOB_RECURSE WPEFRAMEWORK_UNIT_TESTS_SOURCES *.cpp)

set(WPEFRAMEWORK_UNIT_TESTS_INPUT
    ${WPEFRAMEWORK_UNIT_TESTS_SOURCES}
    ${WPEFRAMEWORK_UNIT_TESTS_INCLUDES}
    CACHE INTERNAL "${PROJECT_NAME}: Source files" FORCE)

add_executable(${PROJECT_NAME} ${WPEFRAMEWORK_UNIT_TESTS_INPUT})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${WPEFRAMEWORK_UNIT_TESTS_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${WPEFRAMEWORK_UNIT_TESTS_INCLUDE})
target_link_libraries(${PROJECT_NAME} ${WPEFRAMEWORK_UNIT_TESTS_LIBS})

setup_target_properties_executable(${PROJECT_NAME})

#show_target_properties(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
