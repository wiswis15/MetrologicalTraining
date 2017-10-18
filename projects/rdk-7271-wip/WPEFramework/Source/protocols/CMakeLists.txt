project(WPEFrameworkProtocols)

include(setup_target_properties_library)
include(show_target_properties)

find_package(ZLIB REQUIRED)

set(WPEFRAMEWORK_WEBSOCKET_INCLUDE
    ${ZLIB_INCLUDE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

message("Setting up ${PROJECT_NAME}")

set(WPEFRAMEWORK_PROTOCOLS_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(WPEFRAMEWORK_PROTOCOLS_INCLUDE
    ${ZLIB_INCLUDE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

message("ZLIB_LIBRARIES: " ${ZLIB_LIBRARIES})

set(WPEFRAMEWORK_PROTOCOLS_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${ZLIB_LIBRARIES}
    WPEFrameworkTracing
    WPEFrameworkCore
    WPEFrameworkCryptalgo)

file(GLOB WPEFRAMEWORK_WEBSOCKET_INCLUDES ../websocket/*.h)
#file(GLOB WPEFRAMEWORK_WEBSOCKET_SOURCES ../websocket/*.cpp)
file(GLOB WPEFRAMEWORK_RPC_INCLUDES ../rpc/*.h)
#file(GLOB WPEFRAMEWORK_RPC_SOURCES ../rpc/*.cpp)

# TODO: can we leave out one file ("Module.cpp") when listing files in a GLOB?
set(WPEFRAMEWORK_RPC_SOURCES
    ../rpc/Administrator.cpp
    ../rpc/Communicator.cpp
    ../rpc/ITracing.cpp
    ../rpc/IUnknown.cpp
)

set(WPEFRAMEWORK_WEBSOCKET_SOURCES
    ../websocket/URL.cpp
    ../websocket/WebSerializer.cpp
    ../websocket/WebSocketLink.cpp
)

set(WPEFRAMEWORK_PROTOCOLS_INCLUDES "${WPEFRAMEWORK_WEBSOCKET_INCLUDES} ${WPEFRAMEWORK_RPC_INCLUDES}")
set(WPEFRAMEWORK_PROTOCOLS_SOURCES ${WPEFRAMEWORK_WEBSOCKET_SOURCES})

list(APPEND WPEFRAMEWORK_PROTOCOLS_SOURCES
    ${WPEFRAMEWORK_RPC_SOURCES}
)

#message("WPEFRAMEWORK_PROTOCOLS_SOURCES: " ${WPEFRAMEWORK_PROTOCOLS_SOURCES})

# Remove "Module.cpp"s
#list(REMOVE_ITEM WPEFRAMEWORK_PROTOCOLS_SOURCES "../rpc/Module.cpp")
#list(REMOVE_ITEM WPEFRAMEWORK_PROTOCOLS_SOURCES "../websocket/Module.cpp")

# Add "Module.cpp" from this project
list(APPEND WPEFRAMEWORK_PROTOCOLS_SOURCES "Module.cpp")

if(INSTALL_HEADERS_TO_TARGET)
    install(FILES ${WPEFRAMEWORK_RPC_INCLUDES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/rpc)
    install(FILES ${WPEFRAMEWORK_WEBSOCKET_INCLUDES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/websocket)
else()
    foreach(path ${CMAKE_FIND_ROOT_PATH})
        file(COPY ${WPEFRAMEWORK_WEBSOCKET_INCLUDES} DESTINATION ${path}/usr/include/WPEFramework/websocket)
        file(COPY ${WPEFRAMEWORK_RPC_INCLUDES} DESTINATION ${path}/usr/include/WPEFramework/rpc)
    endforeach()
endif()

set(WPEFRAMEWORK_PROTOCOLS_INPUT
    ${WPEFRAMEWORK_WEBSOCKET_SOURCES}
    ${WPEFRAMEWORK_WEBSOCKET_INCLUDES}
    ${WPEFRAMEWORK_RPC_SOURCES}
    ${WPEFRAMEWORK_RPC_INCLUDES}
    CACHE INTERNAL "${PROJECT_NAME}: Source files" FORCE)

add_library(${PROJECT_NAME} ${WPEFRAMEWORK_PROTOCOLS_SOURCES})
add_definitions(-DMODULE_NAME=WPEFramework_Protocols)
#add_library(${PROJECT_NAME} ${WPEFRAMEWORK_RPC_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${WPEFRAMEWORK_WEBSOCKET_DEFINITIONS})
#target_include_directories(${PROJECT_NAME} PRIVATE ${WPEFRAMEWORK_WEBSOCKET_INCLUDE})
target_link_libraries(${PROJECT_NAME} ${WPEFRAMEWORK_PROTOCOLS_LIBS})
#set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_WEBSOCKET)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${WPEFRAMEWORK_VERSION})

#setup_target_properties_library(${PROJECT_NAME})

#show_target_properties(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
