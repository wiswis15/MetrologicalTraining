include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_WEBSOCKETS_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
)

set (WPEFRAMEWORK_TEST_WEBSOCKETS_SOURCES
       serverclient.cpp
    )

add_executable(websockettest ${WPEFRAMEWORK_TEST_WEBSOCKETS_SOURCES})
target_link_libraries(websockettest ${WPEFRAMEWORK_TEST_WEBSOCKETS_LIBS})

install(TARGETS websockettest DESTINATION bin)
