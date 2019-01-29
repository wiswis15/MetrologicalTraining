cmake_minimum_required(VERSION 2.8)
project(DRMNull2)

set(DRM_PLUGIN_NAME "Null2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_CROSSCOMPILING)
   set(STAGING_DIR "${CMAKE_INSTALL_PREFIX}/..")
else()
   # TODO: this can be multiple directories
   set(STAGING_DIR "${CMAKE_FIND_ROOT_PATH}")
endif()

include_directories(${STAGING_DIR}/usr/include/WPEFramework)

file(GLOB DRM_PLUGIN_INCLUDES *.h)

set(DRM_PLUGIN_LIBS "")

set(DRM_PLUGIN_SOURCES 
    MediaKeySession.cpp)

# add the library
add_library(${DRM_PLUGIN_NAME} SHARED ${DRM_PLUGIN_SOURCES})
target_compile_definitions(${DRM_PLUGIN_NAME} PRIVATE ${PLAYREADY_FLAGS})
target_include_directories(${DRM_PLUGIN_NAME} PRIVATE ${PLAYREADY_INCLUDE_DIRS})
target_link_libraries(${DRM_PLUGIN_NAME} ${DRM_PLUGIN_LIBS})
set_target_properties(${DRM_PLUGIN_NAME} PROPERTIES SUFFIX ".drm")
set_target_properties(${DRM_PLUGIN_NAME} PROPERTIES PREFIX "")

install(TARGETS ${DRM_PLUGIN_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/WPEFramework/OCDM)

