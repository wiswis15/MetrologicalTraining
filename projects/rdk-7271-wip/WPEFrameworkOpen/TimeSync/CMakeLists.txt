find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

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

set(WPEFRAMEWORK_PLUGIN_TIMESYNC_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)    

set(WPEFRAMEWORK_PLUGIN_TIMESYNC_SOURCES 
    Module.cpp
    TimeSync.cpp
    NTPClient.cpp
)
 
# add the library
add_library(WPEFrameworkTimeSync SHARED ${WPEFRAMEWORK_PLUGIN_TIMESYNC_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkTimeSync ${WPEFRAMEWORK_PLUGIN_TIMESYNC_LIBS})
set_target_properties(WPEFrameworkTimeSync PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_TIMESYNC)

install(TARGETS WPEFrameworkTimeSync DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map() 
#    kv(callsign TimeSync)
    kv(locator libWPEFrameworkTimeSync.so)
    kv(classname TimeSync)
    kv(autostart true)
    key(configuration)
end()
ans(WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG)

map()
    kv(system Controller)
    kv(interval 30)
    kv(retries 20)
    kv(periodicity 24)
    key(sources)
end()
ans(WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG_SERVERS)

map_append(${WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG_SERVERS} sources ntp://0.pool.ntp.org)
map_append(${WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG_SERVERS} sources ntp://1.pool.ntp.org)
map_append(${WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG_SERVERS} sources ntp://2.pool.ntp.org)
map_append(${WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG_SERVERS} sources ntp://3.pool.ntp.org)

map_append(${WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG} configuration ${WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG_SERVERS})

json_write("${CMAKE_CURRENT_LIST_DIR}/TimeSync.json" ${WPEFRAMEWORK_PLUGIN_TIMESYNC_CONFIG})
install(FILES TimeSync.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

