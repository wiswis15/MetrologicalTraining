project(cryptalgo)

include(setup_target_properties_library)
include(show_target_properties)

message("Setting up cryptalgo")

set(WPEFRAMEWORK_CRYPTALGO_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(WPEFRAMEWORK_CRYPTALGO_INCLUDE
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(WPEFRAMEWORK_CRYPTALGO_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    WPEFrameworkCore)

file(GLOB WPEFRAMEWORK_CRYPTALGO_INCLUDES *.h)
set(WPEFRAMEWORK_CRYPTALGO_SOURCES
    Hash.cpp
    Random.cpp
	Module.cpp
	AES.cpp
	AESImplementation.cpp)

if(INSTALL_HEADERS_TO_TARGET)
  install(FILES ${WPEFRAMEWORK_CRYPTALGO_INCLUDES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cryptalgo)
else()    
   foreach(path ${CMAKE_FIND_ROOT_PATH})
      file(COPY ${WPEFRAMEWORK_CRYPTALGO_INCLUDES} DESTINATION ${path}/usr/include/WPEFramework/cryptalgo)
    endforeach()
endif()

set(WPEFRAMEWORK_CRYPTALGO_INPUT
    ${WPEFRAMEWORK_CRYPTALGO_SOURCES}
    ${WPEFRAMEWORK_CRYPTALGO_INCLUDES}
    CACHE INTERNAL "${PROJECT_NAME}: Source files" FORCE)

add_library(WPEFrameworkCryptalgo ${WPEFRAMEWORK_CRYPTALGO_INPUT})
target_compile_definitions(WPEFrameworkCryptalgo PRIVATE ${WPEFRAMEWORK_CRYPTALGO_DEFINITIONS})
target_include_directories(WPEFrameworkCryptalgo PRIVATE ${WPEFRAMEWORK_CRYPTALGO_INCLUDE})
target_link_libraries(WPEFrameworkCryptalgo ${WPEFRAMEWORK_CRYPTALGO_LIBS})
set_target_properties(WPEFrameworkCryptalgo PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_CRYPTALGO)
set_target_properties(WPEFrameworkCryptalgo PROPERTIES SOVERSION ${WPEFRAMEWORK_VERSION})

#setup_target_properties_library(WPEFrameworkCryptalgo)

#show_target_properties(${PROJECT_NAME})

install(TARGETS WPEFrameworkCryptalgo DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
