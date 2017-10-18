find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

find_library(
   WPEFRAMEWORKPLUGINS_LIBRARY
   NAMES WPEFrameworkPlugins
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

find_path(
   WPEFRAMEWORKPLUGINS_INCLUDE_DIR
   WPEFrameworkPlugins/plugins.h
   HINTS ${PC_WPEFRAMEWORK_INCLUDEDIR} ${PC_WPEFRAMEWORK_INCLUDE_DIRS}
)

set(WPEFRAMEWORK_PLUGIN_WEBPROXY_LIBS 
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)

set(WPEFRAMEWORK_PLUGIN_WEBPROXY_SOURCES 
    Module.cpp
    WebProxy.cpp
)
 
# add the library
add_library(WPEFrameworkWebProxy SHARED ${WPEFRAMEWORK_PLUGIN_WEBPROXY_SOURCES})
include_directories(${PC_WPEFRAMEWORK_INCLUDEDIR})
target_link_libraries(WPEFrameworkWebProxy ${WPEFRAMEWORK_PLUGIN_WEBPROXY_LIBS})
set_target_properties(WPEFrameworkWebProxy PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_WEBPROXY)

install(TARGETS WPEFrameworkWebProxy DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map() 
#  kv(callsign WebProxy)
  kv(locator libWPEFrameworkWebProxy.so)
  kv(classname WebProxy)
  key(configuration)
  map()
    kv(connections 10)
    key(links)
  end()
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)

map() 
#  kv(callsign WebProxy)
  kv(locator libWPEFrameworkWebProxy.so)
  kv(classname WebProxy)
  key(configuration)
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)

map()
  kv(connections 10)
  key(links)
end()
ans(WPEFRAMEWORK_DEV_CONFIG)

map() 
  kv(name upnp)
  kv(type udp)
  kv(host 239.255.255.250:1900)
end()
ans(UPNP_CONFIG)
map_append(${WPEFRAMEWORK_DEV_CONFIG} links ${UPNP_CONFIG})

map() 
  kv(name serial)
  kv(type serial)
  kv(device "/dev/ttyACM0")
  key(configuration)
  map()
    kv(baudrate 115200)
    kv(parity "none")
    kv(data 8)
    kv(stop 1)
  end()
end()
ans(SERIAL_CONFIG)
map_append(${WPEFRAMEWORK_DEV_CONFIG} links ${SERIAL_CONFIG})

map_append(${WPEFRAMEWORK_PLUGIN_CONFIG} configuration ${WPEFRAMEWORK_DEV_CONFIG})

json_write("${CMAKE_CURRENT_LIST_DIR}/WebProxy.json" ${WPEFRAMEWORK_PLUGIN_CONFIG})
install(FILES WebProxy.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

