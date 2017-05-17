cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(CMAKE_CROSSCOMPILING)
   foreach (searchPath ${CMAKE_FIND_ROOT_PATH})
      if (EXISTS "${searchPath}/usr/share/WPEFramework/cmake/config.cmake")
          include(${searchPath}/usr/share/WPEFramework/cmake/config.cmake)
      endif (EXISTS "${searchPath}/usr/share/WPEFramework/cmake/config.cmake")
   endforeach (path)
else(CMAKE_CROSSCOMPILING)
   include(${CMAKE_INSTALL_PREFIX}/share/WPEFramework/cmake/config.cmake)
endif(CMAKE_CROSSCOMPILING)

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

set(WPEFRAMEWORK_PLUGIN_OUTOFPROCESSTEST_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

set(WPEFRAMEWORK_PLUGIN_OUTOFPROCESSTEST_SOURCES 
    Module.cpp
    OutOfProcessTest.cpp
    OutOfProcessImplementation.cpp
)

message("WPEFRAMEWORKPLUGINS_INCLUDE_DIR: " ${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})

# add the library
add_library(WPEFrameworkOutOfProcessTest SHARED ${WPEFRAMEWORK_PLUGIN_OUTOFPROCESSTEST_SOURCES})
target_link_libraries(WPEFrameworkOutOfProcessTest ${WPEFRAMEWORK_PLUGIN_OUTOFPROCESSTEST_LIBS})
target_include_directories(WPEFrameworkOutOfProcessTest PUBLIC ${WPE_WEBKIT_INCLUDE_DIRS})
set_target_properties(WPEFrameworkOutOfProcessTest PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_OUTOFPROCESSTEST)
install(TARGETS WPEFrameworkOutOfProcessTest DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map() 
#  kv(callsign OutOfProcessTest)
  kv(locator libWPEFrameworkOutOfProcessTest.so)
  kv(classname OutOfProcessTest)
  kv(autostart true)
  key(configuration)
  map()
    kv(test true)
  end()
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/OutOfProcessTest.json" ${WPEFRAMEWORK_PLUGIN_CONFIG})
install(FILES OutOfProcessTest.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

