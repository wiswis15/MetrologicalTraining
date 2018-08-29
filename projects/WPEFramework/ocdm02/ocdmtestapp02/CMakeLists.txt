cmake_minimum_required(VERSION 2.8)
project(ocdmtestapp)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(WPEFramework)

include_directories("${CMAKE_INSTALL_PREFIX}/include"
                    "${CMAKE_INSTALL_PREFIX}/include/WPEFramework/ocdm-accessor"
                    "${WPEFRAMEWORK_INCLUDE_DIRS}")
                    
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

add_executable(ocdmtestapp02
   main.c
)

target_link_libraries(ocdmtestapp02 WPEFrameworkOCDM)

install(TARGETS ocdmtestapp02 DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

