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

find_package(BCMHost REQUIRED)
include_directories (${BCM_HOST_INCLUDE_DIRS})

set(WPEFRAMEWORK_PLUGIN_CAPTURESCREEN_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${LIBGENERICS_LIBRARIES}
    ${LIBTRACING_LIBRARIES}
    ${BCM_HOST_LIBRARIES}
    ${WPEFRAMEWORK_PLUGIN_BASE_LIBRARIES}
)

set(WPEFRAMEWORK_PLUGIN_CAPTURESCREEN_SOURCES 
	${PNG_INCLUDE_DIRS}
    Module.cpp
    Snapshot.cpp
)

find_package(PNG REQUIRED)

# add the library
add_library(WPEFrameworkSnapshot SHARED ${WPEFRAMEWORK_PLUGIN_CAPTURESCREEN_SOURCES})
include_directories(${WPEFRAMEWORKPLUGINS_INCLUDE_DIR})

# Add definitions.
add_definitions(-D USE_VCHIQ_ARM=1)
add_definitions(-DOMX_SKIP64BIT)
add_definitions(-DRPI_NO_X)

target_link_libraries(WPEFrameworkSnapshot ${WPEFRAMEWORK_PLUGIN_CAPTURESCREEN_LIBS}
	${PNG_LIBRARIES}
	-lopenmaxil
	 )

set_target_properties(WPEFrameworkSnapshot PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_WPEFRAMEWORKSUPPORT)

install(TARGETS WPEFrameworkSnapshot DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map() 
#  kv(callsign Snapshot)
  kv(locator libWPEFrameworkSnapshot.so)
  kv(classname Snapshot)
end()
ans(WPEFRAMEWORK_PLUGIN_SNAPSHOT_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/Snapshot.json" ${WPEFRAMEWORK_PLUGIN_SNAPSHOT_CONFIG})
install(FILES Snapshot.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)


