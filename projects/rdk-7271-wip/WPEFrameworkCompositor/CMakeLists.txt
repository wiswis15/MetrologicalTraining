cmake_minimum_required(VERSION 2.8)

set(plugin_name WPEFrameworkCompositor)

# TODO: this is a hack, get it from env, or at least RDK specific
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GNU_SOURCE -DHAVE_CONFIG_H -DWESTEROS_PLATFORM_EMBEDDED -DWESTEROS_HAVE_WAYLAND_EGL -DENABLE_SBPROTOCOL -DUSE_XDG_VERSION4 -DWESTEROS_PLATFORM_NEXUS -DVCX=5 -DBSTD_CPU_ENDIAN=BSTD_ENDIAN_LITTLE -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/interface/khronos/include -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/interface/khronos/include/bcg_abstract -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/vcos/include -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/WPEFramework/ -I/home/sander/Projects/rdk-westeros/build-brcm97271ip-refboard/tmp/sysroots/brcm97271ip-refboard/usr/include/WPEFramework/virtualkeyboard")

if(CMAKE_CROSSCOMPILING)
   foreach (searchPath ${CMAKE_FIND_ROOT_PATH})
      if (EXISTS "${searchPath}/usr/include/WPEFramework/cmake/config.cmake")
          include(${searchPath}/usr/include/WPEFramework/cmake/config.cmake)
      endif (EXISTS "${searchPath}/usr/include/WPEFramework/cmake/config.cmake")
   endforeach ()
else(CMAKE_CROSSCOMPILING)
   include(${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake/config.cmake)
endif(CMAKE_CROSSCOMPILING)

option(WPEFRAMEWORK_PLUGIN_COMPOSITOR_SYSTEM "Callsign to report information." "Controller")
option(WPEFRAMEWORK_PLUGIN_COMPOSITOR_IMPLEMENTATION "Defines what implementation(s) are included." "None")
option(WPEFRAMEWORK_PLUGIN_COMPOSITOR_HARDWAREREADY "Manual overrule of the time it takes to initialisize all hardware (s) [0 - UINT16_MAX]." 0)
option(WPEFRAMEWORK_PLUGIN_COMPOSITOR_SERVICE "Defines where to get resouce handles. [internal, external]." external)
option(WPEFRAMEWORK_PLUGIN_COMPOSITOR_OUTOFPROCESS "Controls if the plugin should run in its own process. [true, false]." true)
option(WPEFRAMEWORK_PLUGIN_COMPOSITOR_AUTOTRACE "Contols if the plugin should automatically start tracing. [ON, OFF]." OFF)
option(WPEFRAMEWORK_PLUGIN_COMPOSITOR_BOXMODE "Allows for selecting a boxmode (Nexus only).")
option(INSTALL_HEADERS_TO_TARGET "Install WPEFramework headers." OFF)

set(WPEFRAMEWORK_PLUGIN_COMPOSITOR_IMPLEMENTATION_LIB "libplatformplugin.so" CACHE STRING "Specify a library with a compositor implentation." )

find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

find_library(
   WPEFRAMEWORKPLUGINS_LIBRARY
   NAMES WPEFrameworkPlugins
   HINTS ${PC_WPEFRAMEWORK_LIBDIR}
)

find_path(
   WPEFRAMEWORKPLUGINS_INCLUDE_DIR
   WPEFramework/plugins/plugins.h
   HINTS ${PC_WPEFRAMEWORK_INCLUDEDIR}
)

include_directories(${PC_WPEFRAMEWORK_INCLUDEDIR})

if (BUILD_REFERENCE)
    add_definitions (-DBUILD_REFERENCE=${BUILD_REFERENCE})
endif()

if(WPEFRAMEWORK_PLUGIN_COMPOSITOR_SERVER)
  add_subdirectory (server)
endif (WPEFRAMEWORK_PLUGIN_COMPOSITOR_SERVER)

add_subdirectory (lib)

if (${WPEFRAMEWORK_PLUGIN_COMPOSITOR_IMPLEMENTATION} STREQUAL "Wayland")
  set(WPEFRAMEWORK_PLUGIN_COMPOSITOR_CLIENT_LIBS waylandeglclient)
endif (${WPEFRAMEWORK_PLUGIN_COMPOSITOR_IMPLEMENTATION} STREQUAL "Wayland")

set(WPEFRAMEWORK_PLUGIN_COMPOSITOR_LIBS
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
    ${WPEFRAMEWORK_PLUGIN_COMPOSITOR_CLIENT_LIBS}
)
set(WPEFRAMEWORK_PLUGIN_COMPOSITOR_SOURCES
    Module.cpp
    Compositor.cpp
)

# add the library
add_library(${plugin_name} SHARED ${WPEFRAMEWORK_PLUGIN_COMPOSITOR_SOURCES})
target_link_libraries(${plugin_name} ${WPEFRAMEWORK_PLUGIN_COMPOSITOR_LIBS})
set_target_properties(${plugin_name} PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGIN_COMPOSITOR)

install(TARGETS ${plugin_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

map()
  kv(outofprocess true)
  kv(system Controller)
  kv(service internal)
  kv(hardwareready 3)
  kv(display "wayland-0")
  kv(renderer "/usr/lib/libwesteros_render_gl.so")
  kv(workdir "/tmp/wayland")
if (WPEFRAMEWORK_PLUGIN_COMPOSITOR_BOXMODE)
  kv(boxmode ${WPEFRAMEWORK_PLUGIN_COMPOSITOR_BOXMODE})
endif(WPEFRAMEWORK_PLUGIN_COMPOSITOR_BOXMODE)
  kv(locator ${WPEFRAMEWORK_PLUGIN_COMPOSITOR_IMPLEMENTATION_LIB})
end()
ans(COMPOSITOR_PLUGIN_CONFIG)

# default configuration
map()
  kv(locator lib${plugin_name}.so)
  kv(classname Compositor)
end()
ans(WPEFRAMEWORK_PLUGIN_CONFIG)

map_append(${WPEFRAMEWORK_PLUGIN_CONFIG} configuration ${COMPOSITOR_PLUGIN_CONFIG})

if (WPEFRAMEWORK_PLUGIN_COMPOSITOR_AUTOTRACE)
  message("-- Auto tracing for ${plugin_name} is enabled")
  map()
  kv(module WPEFramework_Plugin_Compositor)
  kv(category Information)
  kv(enabled true)
  end()
  ans(WPEFRAMEWORK_PLUGIN_CONFIG_TRACE)
  json_write("${CMAKE_CURRENT_LIST_DIR}/CompositorTracing.json" ${WPEFRAMEWORK_PLUGIN_CONFIG_TRACE})
  install(FILES CompositorTracing.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/tracing/ RENAME Compositor.json)

  map()
  kv(module WPEFramework_Plugin_Compositor_Implementation)
  kv(category Information)
  kv(enabled true)
  end()
  ans(WPEFRAMEWORK_PLUGIN_CONFIG_TRACE)
  json_write("${CMAKE_CURRENT_LIST_DIR}/CompositorTracingImplementation.json" ${WPEFRAMEWORK_PLUGIN_CONFIG_TRACE})
  install(FILES CompositorTracingImplementation.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/tracing/ RENAME CompositorImplementation.json)
endif(WPEFRAMEWORK_PLUGIN_COMPOSITOR_AUTOTRACE)

json_write("${CMAKE_CURRENT_LIST_DIR}/Compositor.json" ${WPEFRAMEWORK_PLUGIN_CONFIG})
install(FILES Compositor.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

