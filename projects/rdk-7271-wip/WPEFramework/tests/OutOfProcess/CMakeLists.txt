cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

find_library(
   WPEFRAMEWORKPLUGINS_LIBRARY
   NAMES WPEFrameworkPlugins
   HINTS ${PC_WPEFRAMEWORK_LIBDIR}
)



set(WEBBRIDGE_PLUGIN_OUTOFPROCESSTEST_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${WEBBRIDGE_PLUGIN_BASE_LIBRARIES}
        WPEFrameworkPlugins
        WPEFrameworkCore
        WPEFrameworkTracing
        #WPEFrameworkInterfaces
        )
set(WEBBRIDGE_PLUGIN_OUTOFPROCESSTEST_SOURCES 
    Module.cpp
    OutOfProcessTest.cpp
    OutOfProcessImplementation.cpp
)

add_library(outofprocesstest SHARED ${WEBBRIDGE_PLUGIN_OUTOFPROCESSTEST_SOURCES})
include_directories(${PC_WPEFRAMEWORK_INCLUDEDIR})
target_link_libraries(outofprocesstest ${WEBBRIDGE_PLUGIN_OUTOFPROCESSTEST_LIBS})
target_include_directories(outofprocesstest PUBLIC ${WPE_WEBKIT_INCLUDE_DIRS})
set_target_properties(outofprocesstest PROPERTIES DEFINE_SYMBOL WEBBRIDGE_PLUGIN_OUTOFPROCESSTEST)
install(TARGETS outofprocesstest DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map()
#  kv(callsign Tracing)
kv(locator liboutofprocesstest.so)
kv(classname OutOfProcessTest)
end()
ans(WPEFRAMEWORK_PLUGIN_OUTOFPROCESS_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/OutOfProcess.json" ${WPEFRAMEWORK_PLUGIN_OUTOFPROCESS_CONFIG})
install(FILES OutOfProcess.json DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/WPEFramework/plugins/)
