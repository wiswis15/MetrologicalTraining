cmake_minimum_required(VERSION 2.8)
project(ocdmtestapp)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("${CMAKE_INSTALL_PREFIX}/include"
                    "${CMAKE_INSTALL_PREFIX}/include/WPEFramework"
                    "${CMAKE_INSTALL_PREFIX}/include/opencdm")
                    
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

add_executable(ocdmtestapp04
   main.cpp
)

target_link_libraries(ocdmtestapp04 ocdm WPEFrameworkCore)

install(TARGETS ocdmtestapp04 DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

