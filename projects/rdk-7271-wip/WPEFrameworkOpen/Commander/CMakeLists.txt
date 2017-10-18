find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

find_library(
   WPEFRAMEWORKPLUGINS_LIBRARY
   NAMES WPEFrameworkPlugins
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

find_library(
   WPEFRAMEWORKCORE_LIBRARY
   NAMES WPEFrameworkCore
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

find_library(
   WPEFRAMEWORKTRACING_LIBRARY
   NAMES WPEFrameworkTracing
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

find_path(
   WPEFRAMEWORKPLUGINS_INCLUDE_DIR
   WPEFramework/plugins/plugins.h
   HINTS ${PC_WPEFRAMEWORK_INCLUDEDIR} ${PC_WPEFRAMEWORK_INCLUDE_DIRS}
)

set(WPEFRAMEWORK_PLUGIN_COMMANDER_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${WPEFRAMEWORKCORE_LIBRARY}
    ${WPEFRAMEWORKTRACING_LIBRARY}
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)

set(WPEFRAMEWORK_PLUGIN_COMMANDER_SOURCES 
    Module.cpp
    Commander.cpp
    Commands.cpp
)

find_package(PNG REQUIRED)

# add the library
add_library(WPEFrameworkCommander SHARED ${WPEFRAMEWORK_PLUGIN_COMMANDER_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkCommander ${WPEFRAMEWORK_PLUGIN_COMMANDER_LIBS})

set_target_properties(WPEFrameworkCommander PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_WPEFRAMEWORKSUPPORT)

install(TARGETS WPEFrameworkCommander DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map() 
#  kv(callsign Commander)
  kv(locator libWPEFrameworkCommander.so)
  kv(classname Commander)
end()
ans(WPEFRAMEWORK_PLUGIN_COMMANDER_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/Commander.json" ${WPEFRAMEWORK_PLUGIN_COMMANDER_CONFIG})
install(FILES Commander.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

