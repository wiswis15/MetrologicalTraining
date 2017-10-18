project(rpc)

include(setup_target_properties_library)
include(show_target_properties)

message("Setting up rpc")

set(WPEFRAMEWORK_RPC_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(WPEFRAMEWORK_RPC_INCLUDE
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(WPEFRAMEWORK_RPC_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    WPEFrameworkCore
    WPEFrameworkTracing)

file(GLOB WPEFRAMEWORK_RPC_INCLUDES *.h)
file(GLOB WPEFRAMEWORK_RPC_SOURCES *.cpp)

if(INSTALL_HEADERS_TO_TARGET)
    install(FILES ${WPEFRAMEWORK_RPC_INCLUDES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/rpc)
else()
    foreach(path ${CMAKE_FIND_ROOT_PATH})
        file(COPY ${WPEFRAMEWORK_RPC_INCLUDES} DESTINATION ${path}/usr/include/WPEFramework/rpc)
    endforeach()
endif()

set(WPEFRAMEWORK_RPC_INPUT
    ${WPEFRAMEWORK_RPC_SOURCES}
    ${WPEFRAMEWORK_RPC_INCLUDES}
    CACHE INTERNAL "${PROJECT_NAME}: Source files" FORCE)

add_library(WPEFrameworkRPC ${WPEFRAMEWORK_RPC_INPUT})
target_compile_definitions(WPEFrameworkRPC PRIVATE ${WPEFRAMEWORK_RPC_DEFINITIONS})
target_include_directories(WPEFrameworkRPC PRIVATE ${WPEFRAMEWORK_RPC_INCLUDE})
target_link_libraries(WPEFrameworkRPC ${WPEFRAMEWORK_RPC_LIBS})
set_target_properties(WPEFrameworkRPC PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_RPC)
set_target_properties(WPEFrameworkRPC PROPERTIES SOVERSION ${WPEFRAMEWORK_VERSION})

setup_target_properties_library(WPEFrameworkRPC)

#show_target_properties(${PROJECT_NAME})

install(TARGETS WPEFrameworkRPC DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
