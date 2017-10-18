# TODO make the helpstings more helpfull.
set(WPEFRAMEWORK_PORT 80 CACHE STRING "The port for the webinterface")
set(WPEFRAMEWORK_BINDING "0.0.0.0" CACHE STRING "The binding a")
set(WPEFRAMEWORK_IDLE_TIME 180 CACHE STRING "Idle time")
set(WPEFRAMEWORK_PERSISTENT_PATH "/root" CACHE STRING "Persistent path")
set(WPEFRAMEWORK_DATA_PATH "/usr/share/WPEFramework" CACHE STRING "Data path")
set(WPEFRAMEWORK_SYSTEM_PATH "/usr/lib/wpeframework/plugins" CACHE STRING "System path")
set(WPEFRAMEWORK_WEBSERVER_PATH "/boot/www" CACHE STRING "Root path for the HTTP server")
set(WPEFRAMEWORK_WEBSERVER_PORT 8080 CACHE STRING "Port for the HTTP server")
set(WPEFRAMEWORK_PROXYSTUB_PATH "/usr/lib/wpeframework/proxystubs" CACHE STRING "Proxy stub path")
set(WPEFRAMEWORK_CONFIG_INSTALL_PATH "/etc/wpeframework" CACHE STRING "Install location of the wpeframework configuration")
set(WPEFRAMEWORK_IPV6_SUPPORT false CACHE STRING "Controls if wpeframework should support ipv6")
set(WPEFRAMEWORK_PRIORITY 0 CACHE STRING "Change the nice level [-20 - 20]")
set(WPEFRAMEWORK_POLICY 0 CACHE STRING "NA")
set(WPEFRAMEWORK_OOMADJUST 0 CACHE STRING "Adapt the OOM score [-15 - 15]")
set(WPEFRAMEWORK_STACKSIZE 0 CACHE STRING "Default stack size per thread")


set(WPEFRAMEWORK_PLUGINHOST_LIBS
    WPEFrameworkPlugins
    WPEFrameworkCore
    #WPEFrameworkInterfaces
)
set(WPEFRAMEWORK_PLUGINHOST_SOURCES 
     PluginHost.cpp
     Controller.cpp
     SystemInfo.cpp
     Probe.cpp
     PluginServer.cpp
)

find_package(PkgConfig)
pkg_check_modules(PC_PROVISION QUIET provision)

if (PC_PROVISION_FOUND)
    add_definitions (-DSECURITY_LIBRARY_INCLUDED )
    list_to_string(PC_PROVISION_CFLAGS_OTHER LIBPROVISION_CFLAGS_AS_STRING)
    set(WPEFRAMEWORK_PLUGINHOST_LIBS ${WPEFRAMEWORK_PLUGINHOST_LIBS} ${PC_PROVISION_LIBRARIES})
endif()

# add the library
add_executable(${PROJECT_NAME} ${WPEFRAMEWORK_PLUGINHOST_SOURCES})
target_link_libraries(${PROJECT_NAME} ${WPEFRAMEWORK_PLUGINHOST_LIBS})

if (PC_PROVISION_FOUND)
    if (NOT "${LIBPROVISION_CFLAGS_AS_STRING}" STREQUAL "")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LIBPROVISION_CFLAGS_AS_STRING})
    endif()
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

if (WPEFRAMEWORK_WEB_UI)
   # First make a list of files to install, otherwise you try to install the UI symlink ;-)
   file(GLOB UI UI/*) 
   install(FILES ${UI} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/WPEFramework/Controller/UI)
endif (WPEFRAMEWORK_WEB_UI)

map_set(${WPEFRAMEWORK_CONFIG} port ${WPEFRAMEWORK_PORT})
map_set(${WPEFRAMEWORK_CONFIG} binding ${WPEFRAMEWORK_BINDING})
map_set(${WPEFRAMEWORK_CONFIG} ipv6 ${WPEFRAMEWORK_IPV6_SUPPORT})
map_set(${WPEFRAMEWORK_CONFIG} idletime ${WPEFRAMEWORK_IDLE_TIME})
map_set(${WPEFRAMEWORK_CONFIG} persistentpath ${WPEFRAMEWORK_PERSISTENT_PATH})
map_set(${WPEFRAMEWORK_CONFIG} datapath ${WPEFRAMEWORK_DATA_PATH})
map_set(${WPEFRAMEWORK_CONFIG} systempath ${WPEFRAMEWORK_SYSTEM_PATH})
map_set(${WPEFRAMEWORK_CONFIG} proxystubpath ${WPEFRAMEWORK_PROXYSTUB_PATH})
map_set(${WPEFRAMEWORK_CONFIG} redirect "/Service/Controller/UI")

# default configutation

map()
kv(priority ${WPEFRAMEWORK_PRIORITY})
kv(policy ${WPEFRAMEWORK_POLICY})
kv(oomadjust ${WPEFRAMEWORK_OOMADJUST})
kv(stacksize ${WPEFRAMEWORK_STACKSIZE})
end()
ans(WPEFRAMEWORK_PROCESS_CONFIG)
map_append(${WPEFRAMEWORK_CONFIG} process ${WPEFRAMEWORK_PROCESS_CONFIG})

# default configutation
map()
    kv(callsign Controller)
    key(configuration)
end()
ans(WPEFRAMEWORK_PLUGIN_CONTROLLER)

map()
if(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC)
  kv(location "external")
endif(WPEFRAMEWORK_PLUGIN_LOCATIONSYNC)

if(WPEFRAMEWORK_PLUGIN_COMPOSITOR)
  kv(identifier "external")
  kv(hardware "external")
endif(WPEFRAMEWORK_PLUGIN_COMPOSITOR)

if(WPEFRAMEWORK_PLUGIN_TIMESYNC)
  kv(time "external")
endif(WPEFRAMEWORK_PLUGIN_TIMESYNC)

  key(resumes)
end()
ans(WPEFRAMEWORK_PLUGIN_CONTROLLER_CONFIGURATION)

if (WPEFRAMEWORK_PLUGIN_WEBSERVER OR WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)
    #Add ___array___ to the key to force json array creation.
    map_append(${WPEFRAMEWORK_PLUGIN_CONTROLLER_CONFIGURATION} resumes ___array___)
endif()

#if (WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)
    map_append(${WPEFRAMEWORK_PLUGIN_CONTROLLER_CONFIGURATION} resumes WebKitBrowser)
#endif (WPEFRAMEWORK_PLUGIN_WEBKITBROWSER)

#if (WPEFRAMEWORK_PLUGIN_WEBSERVER)
    map_append(${WPEFRAMEWORK_PLUGIN_CONTROLLER_CONFIGURATION} resumes WebServer)
#endif (WPEFRAMEWORK_PLUGIN_WEBSERVER)

map()
kv(category "Startup")
kv(enabled true)
end()
ans(WPEFRAMEWORK_PLUGIN_STARTUP_TRACING)

map()
kv(category "Shutdown")
kv(enabled true)
end()
ans(WPEFRAMEWORK_PLUGIN_SHUTDOWN_TRACING)

if(NOT WPEFRAMEWORK_VIRTUALINPUT)
    map()
    kv(locator "/dev/uinput")
    kv(type "device")
    end()
    ans(WPEFRAMEWORK_PLUGIN_INPUT_DEVICE)

    map_append(${WPEFRAMEWORK_CONFIG} input ${WPEFRAMEWORK_PLUGIN_INPUT_DEVICE})
endif(NOT WPEFRAMEWORK_VIRTUALINPUT)

map_append(${WPEFRAMEWORK_CONFIG} tracing ${WPEFRAMEWORK_PLUGIN_STARTUP_TRACING})
map_append(${WPEFRAMEWORK_CONFIG} tracing ${WPEFRAMEWORK_PLUGIN_SHUTDOWN_TRACING})
map_append(${WPEFRAMEWORK_PLUGIN_CONTROLLER} configuration ${WPEFRAMEWORK_PLUGIN_CONTROLLER_CONFIGURATION})
map_append(${WPEFRAMEWORK_CONFIG} plugins ${WPEFRAMEWORK_PLUGIN_CONTROLLER})


