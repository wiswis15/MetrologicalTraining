option(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_STUBBED "Include the wpe browser as a stubbed plugin." OFF)
option(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_YOUTUBE "Include YouTube in seperate plugin." OFF)

set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_AUTOSTART false CACHE STRING true)
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_TRANSPARENT false CACHE STRING true)
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_STARTURL false CACHE STRING "about:blank")
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_USERAGENT false CACHE STRING "Mozilla/5.0 (Macintosh, Intel Mac OS X 10_11_4) AppleWebKit/602.1.28+ (KHTML, like Gecko) Version/9.1 Safari/601.5.17")
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEMORYPROFILE "512m" CACHE STRING "Memory Profile")
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEMORYPRESSURE "databaseprocess:50m,networkprocess:100m,webprocess:300m,rpcprocess:50m" CACHE STRING "Memory Pressure")
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEDIADISKCACHE false CACHE STRING "Media Disk Cache")
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_DISKCACHE "0" CACHE STRING "Disk Cache")
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_XHRCACHE true CACHE STRING "XHR Cache")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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

set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${LIBGENERICS_LIBRARIES}
    ${LIBTRACING_LIBRARIES}
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)

set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_SOURCES 
    Module.cpp
    WebKitBrowser.cpp
    WebKitImplementation.cpp
)

find_package(GLIB REQUIRED)

pkg_check_modules(WPE_WEBKIT wpe-webkit)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(WPE_WEBKIT DEFAULT_MSG WPE_WEBKIT_LIBRARIES)

mark_as_advanced(WPE_WEBKIT_INCLUDE_DIRS WPE_WEBKIT_LIBRARIES)

file(WRITE ${CMAKE_BINARY_DIR}/test_atomic.cpp
"#include <atomic>\n"
"int main() { std::atomic<int64_t> i(0); i++; return 0; }\n")
try_compile(ATOMIC_BUILD_SUCCEEDED ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/test_atomic.cpp)
if (NOT ATOMIC_BUILD_SUCCEEDED)
    list(APPEND WPE_WEBKIT_LIBRARIES atomic)
endif ()
file(REMOVE ${CMAKE_BINARY_DIR}/test_atomic.cpp)

set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_SOURCES 
    ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_SOURCES}
    InjectedBundle/WhiteListedOriginDomainsList.cpp
    InjectedBundle/Utils.cpp
)
set(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_LIBS 
    ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_LIBS}
    ${WPE_WEBKIT_LIBRARIES}
    ${GLIB_LIBRARIES}
    )
 
# add the library
add_library(WPEFrameworkWebKitBrowser SHARED ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
target_link_libraries(WPEFrameworkWebKitBrowser ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_LIBS})
target_include_directories(WPEFrameworkWebKitBrowser PUBLIC ${WPE_WEBKIT_INCLUDE_DIRS})
set_target_properties(WPEFrameworkWebKitBrowser PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)
if(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_STUBBED)
    set_target_properties(WPEFrameworkWebKitBrowser PROPERTIES DEFINE_SYMBOL __STUBBED__)
endif()
install(TARGETS WPEFrameworkWebKitBrowser DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

if(NOT WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_STUBBED)
    # also build injected bundle
    add_subdirectory(InjectedBundle)
endif()

# default configutation
map()
  #kv(callsign WebKitBrowser)
  kv(locator libWPEFrameworkWebKitBrowser.so)
  kv(classname WebKitBrowser)
  kv(autostart ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_AUTOSTART})
  key(configuration)
  map()
    kv(url ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_STARTURL})
    kv(useragent ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_USERAGENT})
    kv(injectedbundle "libWPEInjectedBundle.so")
    kv(transparent ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_TRANSPARENT})
    kv(compositor "noaa")
    kv(inspector "0.0.0.0:9998")
    kv(fps true)
    kv(cursor false)
    kv(touch false)
    kv(msebuffers "audio:2m,video:15m,text:1m")
    kv(memoryprofile ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEMORYPROFILE})
    kv(memorypressure ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEMORYPRESSURE})
    kv(mediadiskcache ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEDIADISKCACHE})
    kv(diskcache ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_DISKCACHE})
    kv(xhrcache ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_XHRCACHE})
  end()
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)
json_write("${CMAKE_CURRENT_LIST_DIR}/WebKitBrowser.json" ${WPEFRAMEWORK_PLUGIN_CONFIG})
install(FILES WebKitBrowser.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

# youtube configuration
if(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_YOUTUBE)
map()
  #kv(callsign YouTube)
  kv(locator libWPEFrameworkWebKitBrowser.so)
  kv(classname WebKitBrowser)
  kv(autostart false)
  key(configuration)
  map()
    kv(url "about:blank")
    kv(useragent ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_USERAGENT})
    kv(injectedbundle "libWPEInjectedBundle.so")
    kv(transparent ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_TRANSPARENT})
    kv(compositor "noaa")
    kv(inspector "0.0.0.0:9999")
    kv(fps true)
    kv(cursor false)
    kv(touch false)
    kv(msebuffers "audio:2m,video:12m,text:1m")
    kv(memoryprofile ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEMORYPROFILE})
    kv(memorypressure ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEMORYPRESSURE})
    kv(mediadiskcache ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_MEDIADISKCACHE})
    kv(diskcache ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_DISKCACHE})
    kv(xhrcache ${WPEFRAMEWORK_PLUGIN_WEBKITBROWSER_XHRCACHE})
  end()
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)
json_write("${CMAKE_CURRENT_LIST_DIR}/YouTube.json" ${WPEFRAMEWORK_PLUGIN_CONFIG})
install(FILES YouTube.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)
endif()
