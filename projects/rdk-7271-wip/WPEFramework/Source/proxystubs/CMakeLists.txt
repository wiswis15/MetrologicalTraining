set(WPEFRAMEWORK_PROXYSTUBS_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    WPEFrameworkProtocols
)
set(WPEFRAMEWORK_PROXYSTUBS_SOURCES 
    PluginInterface.cpp
    Module.cpp
)

# add the library
add_library(proxystubs ${WPEFRAMEWORK_PROXYSTUBS_SOURCES})
target_link_libraries(proxystubs ${WPEFRAMEWORK_PROXYSTUBS_LIBS})
set_target_properties(proxystubs PROPERTIES OUTPUT_NAME WPEFrameworkProxyStubs)

install(TARGETS proxystubs DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/proxystubs)

