find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

option(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_URI "URI to request the location infomation" location.webplatformforembedded.org)

find_library(
   WPEFRAMEWORKPLUGINS_LIBRARY
   NAMES WPEFrameworkPlugins
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

find_path(
   WPEFRAMEWORKPLUGINS_INCLUDE_DIR
   WPEFramework/plugins/plugins.h
   HINTS ${PC_WPEFRAMEWORK_INCLUDEDIR} ${PC_WPEFRAMEWORK_INCLUDE_DIRS}
)

set(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)    

set(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_SOURCES
    Module.cpp
    LocationSync.cpp
    LocationService.cpp
)
 
# add the library
add_library(WPEFrameworkLocationSync SHARED ${WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkLocationSync ${WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_LIBS})
set_target_properties(WPEFrameworkLocationSync PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_LOCATIONSYNC)

install(TARGETS WPEFrameworkLocationSync DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

message("WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_URI: ${WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_URI}")

# default configutation
map() 
#    kv(callsign LocationSync)
    kv(locator libWPEFrameworkLocationSync.so)
    kv(classname LocationSync)
    key(configuration)
    map()
      kv(system Controller)
      kv(interval 10)
      kv(retries 20)
      kv(source ${WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_URI}:80)
    end()
end()
ans(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/LocationSync.json" ${WPEFRAMEWORK_PLUGIN_LOCATIONSYNC_CONFIG})
install(FILES LocationSync.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

