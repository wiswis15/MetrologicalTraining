include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )
include_directories( ${WPEFRAMEWORK_WEBSOCKET_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_WEBINSPECTOR_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
        websocket 
        generics
)

set (WPEFRAMEWORK_TEST_WEBINSPECTOR_SOURCES
        WebInspector.cpp
)

add_executable(WebInspector ${WPEFRAMEWORK_TEST_WEBINSPECTOR_SOURCES})
target_link_libraries(WebInspector ${WPEFRAMEWORK_TEST_WEBINSPECTOR_LIBS})

install(TARGETS WebInspector DESTINATION bin)
