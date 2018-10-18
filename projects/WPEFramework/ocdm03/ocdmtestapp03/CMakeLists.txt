cmake_minimum_required(VERSION 2.8)
project(ocdmtestapp)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(WPEFramework)

include_directories("${CMAKE_INSTALL_PREFIX}/include"
                    "${CMAKE_INSTALL_PREFIX}/include/WPEFramework"
                    "${CMAKE_INSTALL_PREFIX}/include/opencdm"
                    "${WPEFRAMEWORK_INCLUDE_DIRS}")
                    
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

add_executable(ocdmtestapp03
   main.c
)

target_link_libraries(ocdmtestapp03 ocdm)

install(TARGETS ocdmtestapp03 DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

