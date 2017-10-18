project(websocket)

include(setup_target_properties_library)
include(show_target_properties)

message("Setting up websocket")

set(WPEFRAMEWORK_WEBSOCKET_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(WPEFRAMEWORK_WEBSOCKET_INCLUDE
    ${ZLIB_INCLUDE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(WPEFRAMEWORK_WEBSOCKET_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${ZLIB_LIBRARIES}
    WPEFrameworkCore
    WPEFrameworkCryptalgo)

file(GLOB WPEFRAMEWORK_WEBSOCKET_INCLUDES *.h)
file(GLOB WPEFRAMEWORK_WEBSOCKET_SOURCES *.cpp)

if(INSTALL_HEADERS_TO_TARGET)
    install(FILES ${WPEFRAMEWORK_WEBSOCKET_INCLUDES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/websocket)
else()
    foreach(path ${CMAKE_FIND_ROOT_PATH})
        file(COPY ${WPEFRAMEWORK_WEBSOCKET_INCLUDES} DESTINATION ${path}/usr/include/WPEFramework/websocket)
    endforeach()
endif()

set(WPEFRAMEWORK_WEBSOCKET_INPUT
    ${WPEFRAMEWORK_WEBSOCKET_SOURCES}
    ${WPEFRAMEWORK_WEBSOCKET_INCLUDES}
    CACHE INTERNAL "${PROJECT_NAME}: Source files" FORCE)

add_library(WPEFrameworkWebSocket ${WPEFRAMEWORK_WEBSOCKET_INPUT})
target_compile_definitions(WPEFrameworkWebSocket PRIVATE ${WPEFRAMEWORK_WEBSOCKET_DEFINITIONS})
target_include_directories(WPEFrameworkWebSocket PRIVATE ${WPEFRAMEWORK_WEBSOCKET_INCLUDE})
target_link_libraries(WPEFrameworkWebSocket ${WPEFRAMEWORK_WEBSOCKET_LIBS})
set_target_properties(WPEFrameworkWebSocket PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_WEBSOCKET)
set_target_properties(WPEFrameworkWebSocket PROPERTIES SOVERSION ${WPEFRAMEWORK_VERSION})

setup_target_properties_library(WPEFrameworkWebSocket)

#show_target_properties(${PROJECT_NAME})

install(TARGETS WPEFrameworkWebSocket DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
