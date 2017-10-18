set(PLUGINS_INTERFACES_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    WPEFrameworkProtocols
)
set(PLUGINS_INTERFACES_SOURCES 
    ProxyStubs.cpp
)

# add the library
add_library(WPEFrameworkInterfaces ${PLUGINS_INTERFACES_SOURCES})
target_link_libraries(WPEFrameworkInterfaces ${PLUGINS_INTERFACES_LIBS})

file(GLOB WPEFRAMEWORK_INTERACES_INCLUDES *.h)

if(INSTALL_HEADERS_TO_TARGET)
    install(FILES ${WPEFRAMEWORK_INTERACES_INCLUDES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/interfaces)
endif(INSTALL_HEADERS_TO_TARGET)

install(TARGETS WPEFrameworkInterfaces DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/proxystubs)

