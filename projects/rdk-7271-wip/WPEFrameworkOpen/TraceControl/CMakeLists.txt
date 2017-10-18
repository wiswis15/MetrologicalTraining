set(WPEFRAMEWORK_PLUGIN_TRACECONTROL_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${WPEFRAMEWORKPLUGINS_LIBRARY}
)

set(WPEFRAMEWORK_PLUGIN_TRACECONTROL_SOURCES 
    Module.cpp
    TraceControl.cpp
)
 
# add the library
add_library(WPEFrameworkTraceControl SHARED ${WPEFRAMEWORK_PLUGIN_TRACECONTROL_SOURCES})
target_link_libraries(WPEFrameworkTraceControl ${WPEFRAMEWORK_PLUGIN_TRACECONTROL_LIBS})

install(TARGETS WPEFrameworkTraceControl DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins)

# default configutation
map() 
#  kv(callsign Tracing)
  kv(locator libWPEFrameworkTraceControl.so)
  kv(classname TraceControl)
end()
ans(WPEFRAMEWORK_PLUGIN_TRACECONTROL_CONFIG)

json_write("${CMAKE_CURRENT_LIST_DIR}/Tracing.json" ${WPEFRAMEWORK_PLUGIN_TRACECONTROL_CONFIG})
install(FILES Tracing.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/plugins/)

