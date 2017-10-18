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

set(WPEFRAMEWORK_PLUGIN_WEBSHELL_LIBS 
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)

set(WPEFRAMEWORK_PLUGIN_WEBSHELL_SOURCES 
    Module.cpp
    WebShell.cpp
)
 
# add the library
add_library(WPEFrameworkWebShell SHARED ${WPEFRAMEWORK_PLUGIN_WEBSHELL_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkWebShell ${WPEFRAMEWORK_PLUGIN_WEBSHELL_LIBS})
set_target_properties(WPEFrameworkWebShell PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_WEBSHELL)

install(TARGETS WPEFrameworkWebShell DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map() 
#  kv(callsign WebShell)
  kv(locator libWPEFrameworkWebShell.so)
  kv(classname WebShell)
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/WebShell.json" ${WPEFRAMEWORK_PLUGIN_CONFIG})
install(FILES WebShell.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

