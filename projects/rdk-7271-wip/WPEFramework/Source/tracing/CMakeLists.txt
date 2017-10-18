project(tracing)

include(setup_target_properties_library)
include(show_target_properties)

message("Setting up tracing")

set(WPEFRAMEWORK_TRACING_DEFINITIONS
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(WPEFRAMEWORK_TRACING_INCLUDE
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(WPEFRAMEWORK_TRACING_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    WPEFrameworkCore)

file(GLOB WPEFRAMEWORK_TRACING_INCLUDES *.h)
file(GLOB WPEFRAMEWORK_TRACING_SOURCES *.cpp)

if(INSTALL_HEADERS_TO_TARGET)
    install(FILES ${WPEFRAMEWORK_TRACING_INCLUDES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/tracing)
else()
    foreach(path ${CMAKE_FIND_ROOT_PATH})
        file(COPY ${WPEFRAMEWORK_TRACING_INCLUDES} DESTINATION ${path}/usr/include/WPEFramework/tracing)
    endforeach()
endif()

set(WPEFRAMEWORK_TRACING_INPUT
    ${WPEFRAMEWORK_TRACING_SOURCES}
    ${WPEFRAMEWORK_TRACING_INCLUDES}
    CACHE INTERNAL "${PROJECT_NAME}: Source files" FORCE)

add_library(WPEFrameworkTracing ${WPEFRAMEWORK_TRACING_INPUT})
target_compile_definitions(WPEFrameworkTracing PRIVATE ${WPEFRAMEWORK_TRACING_DEFINITIONS})
target_include_directories(WPEFrameworkTracing PRIVATE ${WPEFRAMEWORK_TRACING_INCLUDE})
target_link_libraries(WPEFrameworkTracing ${WPEFRAMEWORK_TRACING_LIBS})
set_target_properties(WPEFrameworkTracing PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_TRACING)
set_target_properties(WPEFrameworkTracing PROPERTIES SOVERSION ${WPEFRAMEWORK_VERSION})

#setup_target_properties_library(WPEFrameworkTracing)

#show_target_properties(${PROJECT_NAME})

install(TARGETS WPEFrameworkTracing DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
