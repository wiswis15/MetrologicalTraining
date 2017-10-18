cmake_minimum_required(VERSION 2.8)

#TODO: also here a hack, just like compositor plugin. Needs to be more RDK friendly.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GNU_SOURCE -DHAVE_CONFIG_H -DWESTEROS_PLATFORM_EMBEDDED -DWESTEROS_HAVE_WAYLAND_EGL -DENABLE_SBPROTOCOL -DUSE_XDG_VERSION4 -DWESTEROS_PLATFORM_NEXUS -DVCX=5 -DBSTD_CPU_ENDIAN=BSTD_ENDIAN_LITTLE -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/interface/khronos/include -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/interface/khronos/include/bcg_abstract -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/vcos/include -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/WPEFramework/ -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/WPEFramework/virtualkeyboard")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(CMAKE_CROSSCOMPILING)
   foreach (searchPath ${CMAKE_FIND_ROOT_PATH})
      if (EXISTS "${searchPath}/usr/include/WPEFramework/cmake/config.cmake")
          include(${searchPath}/usr/include/WPEFramework/cmake/config.cmake)
      endif (EXISTS "${searchPath}/usr/include/WPEFramework/cmake/config.cmake")
   endforeach ()
else(CMAKE_CROSSCOMPILING)
   include(${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake/config.cmake)
endif(CMAKE_CROSSCOMPILING)

find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

find_library(
   WPEFRAMEWORK_PLUGINS_LIBRARY
   NAMES WPEFrameworkPlugins
   HINTS ${PC_WPEFRAMEWORK_LIBDIR}
)

find_path(
   WPEFRAMEWORKPLUGINS_INCLUDE_DIR
   plugins/plugins.h
   HINTS ${PC_WPEFRAMEWORK_INCLUDEDIR}
)

option(WPEFRAMEWORK_PLUGIN_COMMANDER "Include Commander plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_DEVICEINFO "Include DeviceInfo plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC "Include LocationSync plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_MONITOR "Include Monitor plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_REMOTECONTROL "Include RemoteControl plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_SNAPSHOT "Include Snapshot plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_TIMESYNC "Include TimeSync plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_TRACECONTROL "Include TraceControl plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER "Include WebKitBrowser plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_WEBPROXY "Include WebProxy plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_WEBSERVER "Include WebServer plugin" OFF)
option(WPEFRAMEWORK_PLUGIN_WEBSHELL "Include WebShell plugin" OFF)
include_directories(${PC_WPEFRAMEWORK_INCLUDEDIR})

if (BUILD_REFERENCE)
    add_definitions (-DBUILD_REFERENCE=${BUILD_REFERENCE})
endif()

if(WPEFRAMEWORK_PLUGIN_COMMANDER)
	add_subdirectory (Commander)
endif(WPEFRAMEWORK_PLUGIN_COMMANDER)

if(WPEFRAMEWORK_PLUGIN_DEVICEINFO)
	add_subdirectory (DeviceInfo)
endif(WPEFRAMEWORK_PLUGIN_DEVICEINFO)

if(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC)
	add_subdirectory (LocationSync)
endif(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC)

if(WPEFRAMEWORK_PLUGIN_MONITOR)
	add_subdirectory (Monitor)
endif(WPEFRAMEWORK_PLUGIN_MONITOR)

if(WPEFRAMEWORK_PLUGIN_REMOTECONTROL)
	add_subdirectory (RemoteControl)
endif(WPEFRAMEWORK_PLUGIN_REMOTECONTROL)

if(WPEFRAMEWORK_PLUGIN_SNAPSHOT)
	add_subdirectory (Snapshot)
endif(WPEFRAMEWORK_PLUGIN_SNAPSHOT)

if(WPEFRAMEWORK_PLUGIN_TIMESYNC)
	add_subdirectory (TimeSync)
endif(WPEFRAMEWORK_PLUGIN_TIMESYNC)

if(WPEFRAMEWORK_PLUGIN_TRACECONTROL)
	add_subdirectory (TraceControl)
endif(WPEFRAMEWORK_PLUGIN_TRACECONTROL)

if(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)
	add_subdirectory (WebKitBrowser)
endif(WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)

if(WPEFRAMEWORK_PLUGIN_WEBPROXY)
	add_subdirectory (WebProxy)
endif(WPEFRAMEWORK_PLUGIN_WEBPROXY)

if(WPEFRAMEWORK_PLUGIN_WEBSERVER)
	add_subdirectory (WebServer)
endif(WPEFRAMEWORK_PLUGIN_WEBSERVER)

if(WPEFRAMEWORK_PLUGIN_WEBSHELL)
	add_subdirectory (WebShell)
endif(WPEFRAMEWORK_PLUGIN_WEBSHELL)

