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

set(WPEFRAMEWORK_PLUGIN_MONITOR_WEBKITBROWSER_MEMORYLIMIT 307200 CACHE STRING "WebKitBrowser memory limit")
set(WPEFRAMEWORK_PLUGIN_MONITOR_YOUTUBE_MEMORYLIMIT 307200 CACHE STRING "YouTube memory limit")
set(WPEFRAMEWORK_PLUGIN_MONITOR_NETFLIX_MEMORYLIMIT 307200 CACHE STRING "Netflix memory limit")

set(WPEFRAMEWORK_PLUGIN_MONITOR_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)

set(WPEFRAMEWORK_PLUGIN_MONITOR_SOURCES 
    Module.cpp
    Monitor.cpp
)
 
# add the library
add_library(WPEFrameworkMonitor SHARED ${WPEFRAMEWORK_PLUGIN_MONITOR_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkMonitor ${WPEFRAMEWORK_PLUGIN_MONITOR_LIBS})

install(TARGETS WPEFrameworkMonitor DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

map()
#  kv(callsign Monitor)
  kv(locator libWPEFrameworkMonitor.so)
  kv(classname Monitor)
  kv(autostart true)
  key(configuration)
end()
ans(MONITOR_PLUGIN_CONFIG)

map()
  key(observables)
end()
ans(MONITOR_OBSERVABLES_CONFIG)

if(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)
    map()
        kv(callsign WebKitBrowser)
        kv(memory 5)
        kv(memorylimit ${WPEFRAMEWORK_PLUGIN_MONITOR_WEBKITBROWSER_MEMORYLIMIT})
        kv(operational 1)
    end()
    ans(WEBKITBROWSER_MONITOR_CONFIG)
    map_append(${MONITOR_OBSERVABLES_CONFIG} observables ${WEBKITBROWSER_MONITOR_CONFIG})
endif(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)

if(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_YOUTUBE)
    map()
        kv(callsign YouTube)
        kv(memory 5)
        kv(memorylimit ${WPEFRAMEWORK_PLUGIN_MONITOR_YOUTUBE_MEMORYLIMIT})
        kv(operational 1)
    end()
    ans(YOUTUBE_MONITOR_CONFIG)
    map_append(${MONITOR_OBSERVABLES_CONFIG} observables ${YOUTUBE_MONITOR_CONFIG})
endif(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_YOUTUBE)

if(WPEFRAMEWORK_PLUGIN_NETFLIX)
    map()
        kv(callsign Netflix)
        kv(memory 5)
        kv(memorylimit ${WPEFRAMEWORK_PLUGIN_MONITOR_NETFLIX_MEMORYLIMIT})
    end()
    ans(NETFLIX_MONITOR_CONFIG)
    map_append(${MONITOR_OBSERVABLES_CONFIG} observables ${NETFLIX_MONITOR_CONFIG})
endif(WPEFRAMEWORK_PLUGIN_NETFLIX)

if(WPEFRAMEWORK_PLUGIN_OUTOFPROCESSTEST)
    map()
        kv(callsign OutOfProcessTest)
        kv(memory 5)
        kv(memorylimit 3000)
        kv(operational 1)
    end()
    ans(OUTOFPROCESSTEST_MONITOR_CONFIG)
    map_append(${MONITOR_OBSERVABLES_CONFIG} observables ${OUTOFPROCESSTEST_MONITOR_CONFIG})
endif(WPEFRAMEWORK_PLUGIN_OUTOFPROCESSTEST)

map_append(${MONITOR_PLUGIN_CONFIG} configuration ${MONITOR_OBSERVABLES_CONFIG})

json_write("${CMAKE_CURRENT_LIST_DIR}/Monitor.json" ${MONITOR_PLUGIN_CONFIG})
install(FILES Monitor.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

