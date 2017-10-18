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

set(WPEFRAMEWORK_PLUGIN_DEVICEINFO_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)    

set(WPEFRAMEWORK_PLUGIN_DEVICEINFO_SOURCES 
    Module.cpp
    DeviceInfo.cpp
)
 
# add the library
add_library(WPEFrameworkDeviceInfo SHARED ${WPEFRAMEWORK_PLUGIN_DEVICEINFO_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkDeviceInfo ${WPEFRAMEWORK_PLUGIN_DEVICEINFO_LIBS})
set_target_properties(WPEFrameworkDeviceInfo PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_DEVICEINFO)

install(TARGETS WPEFrameworkDeviceInfo DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configuration
map() 
#    kv(callsign "DeviceInfo")
    kv(locator "libWPEFrameworkDeviceInfo.so")
    kv(classname "DeviceInfo")
end()
ans(WPEFRAMEWORK_PLUGIN_DEVICEINFO_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/DeviceInfo.json" ${WPEFRAMEWORK_PLUGIN_DEVICEINFO_CONFIG})
install(FILES DeviceInfo.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

