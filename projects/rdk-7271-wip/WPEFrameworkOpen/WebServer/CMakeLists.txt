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

set(WPEFRAMEWORK_PLUGIN_WEBSERVER_LIBS 
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)

set(WPEFRAMEWORK_PLUGIN_WEBSERVER_SOURCES 
    Module.cpp
    WebServerImplementation.cpp
    WebServer.cpp
)
 
# add the library
add_library(WPEFrameworkWebServer SHARED ${WPEFRAMEWORK_PLUGIN_WEBSERVER_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkWebServer ${WPEFRAMEWORK_PLUGIN_WEBSERVER_LIBS})
set_target_properties(WPEFrameworkWebServer PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_WEBSERVER)

install(TARGETS WPEFrameworkWebServer DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

function(add_app map)
  map()
    kv(name ${ARGV0})
    kv(allowstop ${ARGV1})
    kv(url ${ARGV2})
    if(ARGV3)
      kv(additionaldata ${ARGV3})
    endif()
  end()
endfunction(add_app)     

# default configutation
map() 
#  kv(callsign WebServer)
  kv(locator libWPEFrameworkWebServer.so)
  kv(classname WebServer)
  key(configuration)
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)

map()
  kv(port ${WPEFRAMEWORK_WEBSERVER_PORT})
  kv(binding "0.0.0.0")
  kv(path ${WPEFRAMEWORK_WEBSERVER_PATH})
end()
ans(WPEFRAMEWORK_WEBSERVER_CONFIGURATION)

if(WPEFRAMEWORK_PLUGIN_DEVICEINFO)
map()
  kv(path /Service/DeviceInfo)
  kv(subst /Service/DeviceInfo)
  kv(server 127.0.0.1:${WPEFRAMEWORK_PORT})
end()
ans(DEVICEINFO_CONFIG)
map_append(${WPEFRAMEWORK_WEBSERVER_CONFIGURATION} proxies ${DEVICEINFO_CONFIG})
endif(WPEFRAMEWORK_PLUGIN_DEVICEINFO)

if(WPEFRAMEWORK_PLUGIN_DIALSERVER)
map()
  kv(path /Service/DIALServer)
  kv(subst /Service/DIALServer)
  kv(server 127.0.0.1:${WPEFRAMEWORK_PORT})
end()
ans(DIALSERVER_CONFIG)
map_append(${WPEFRAMEWORK_WEBSERVER_CONFIGURATION} proxies ${DIALSERVER_CONFIG})
endif(WPEFRAMEWORK_PLUGIN_DIALSERVER)

map_append(${WPEFRAMEWORK_PLUGIN_CONFIG} configuration ${WPEFRAMEWORK_WEBSERVER_CONFIGURATION})
#map_append(${WPEFRAMEWORK_CONFIG} plugins ${WPEFRAMEWORK_PLUGIN_CONFIG})

json_write("${CMAKE_CURRENT_LIST_DIR}/WebServer.json" ${WPEFRAMEWORK_PLUGIN_CONFIG})
install(FILES WebServer.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

