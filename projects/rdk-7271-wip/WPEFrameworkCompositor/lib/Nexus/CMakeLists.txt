set(target_name platformplugin)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(WPEFRAMEWORK_PLATFORM_PLUGIN_LIBS
        WPEFrameworkPlugins
        )

find_package(BcmNxServer REQUIRED)
include_directories(${LIBNXSERVER_INCLUDE_DIRS})

set(WPEFRAMEWORK_PLATFORM_PLUGIN_SOURCES Nexus.cpp)
list(APPEND WPEFRAMEWORK_PLATFORM_PLUGIN_LIBS ${LIBNXSERVER_LIBRARIES})

# add the platformplugin library
add_library(${target_name} ${WPEFRAMEWORK_PLATFORM_PLUGIN_SOURCES})
target_link_libraries(${target_name} ${WPEFRAMEWORK_PLATFORM_PLUGIN_LIBS})
set_target_properties(${target_name} PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLATFORM_PLUGIN)
install(TARGETS ${target_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/WPEFramework/Compositor)
