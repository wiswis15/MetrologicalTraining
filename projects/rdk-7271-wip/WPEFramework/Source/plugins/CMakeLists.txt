# package config
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/wpeframework.pc.in" 
    "${CMAKE_BINARY_DIR}/wpeframework.pc"
    @ONLY)

set(WPEFRAMEWORK_PLUGINS_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    WPEFrameworkCore
    WPEFrameworkTracing
    WPEFrameworkCryptalgo
    WPEFrameworkProtocols
)
set(WPEFRAMEWORK_PLUGINS_SOURCES 
    MetaData.cpp
    Channel.cpp
    Service.cpp
    Logging.cpp
    VirtualInput.cpp
    StateControl.cpp
    Shell.cpp
	Module.cpp
#    ../interfaces/ProxyStubs.cpp
)

file (GLOB_RECURSE FILE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.h) 

if (INSTALL_HEADERS_TO_TARGET)
  install (FILES ${FILE_LIST} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/plugins )
else()    
   foreach (path ${CMAKE_FIND_ROOT_PATH})
      file (COPY ${FILE_LIST} DESTINATION ${path}/usr/include/WPEFramework )
    endforeach (path)
endif (INSTALL_HEADERS_TO_TARGET)

# add the library
add_library(WPEFrameworkPlugins ${WPEFRAMEWORK_PLUGINS_SOURCES})
target_link_libraries(WPEFrameworkPlugins ${WPEFRAMEWORK_PLUGINS_LIBS})
set_target_properties(WPEFrameworkPlugins PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_PLUGINS)

#TODO: this needs to be put back in variables
set_target_properties(WPEFrameworkPlugins PROPERTIES VERSION "1.0.0" SOVERSION "1")
 
install(TARGETS WPEFrameworkPlugins DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
