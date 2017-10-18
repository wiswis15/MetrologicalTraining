set(target_name platformplugin)

find_library(
   WPEFRAMEWORKPLUGINS_LIBRARY
   NAMES WPEFrameworkPlugins
   HINTS ${PC_WPEFRAMEWORK_LIBDIR}
)

set(WPEFRAMEWORK_PLATFORM_PLUGIN_LIBS ${WPEFRAMEWORKPLUGINS_LIBRARY})
set(WPEFRAMEWORK_PLATFORM_PLUGIN_SOURCES Stub.cpp)

# add the platformplugin library
add_library(${target_name} SHARED ${WPEFRAMEWORK_PLATFORM_PLUGIN_SOURCES})
target_include_directories(${target_name} PRIVATE WPEFramework/interfaces)
target_link_libraries(${target_name} ${WPEFRAMEWORK_PLATFORM_PLUGIN_LIBS})
set_target_properties(${target_name} PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLATFORM_PLUGIN)
install(TARGETS ${target_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/WPEFramework/Compositor)
