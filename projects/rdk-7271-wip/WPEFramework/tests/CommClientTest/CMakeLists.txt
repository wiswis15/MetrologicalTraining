include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_COMMCLIENT_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
       rpc
)

# Server
set (WPEFRAMEWORK_TEST_COMMCLIENT_SERVER_SOURCES
    server.cpp
)

add_executable(CommClientTestServer ${WPEFRAMEWORK_TEST_COMMCLIENT_SERVER_SOURCES})
target_link_libraries(CommClientTestServer ${WPEFRAMEWORK_TEST_COMMCLIENT_LIBS})

install(TARGETS CommClientTestServer DESTINATION bin)

# Client
set (WPEFRAMEWORK_TEST_COMMCLIENT_CLIENT_SOURCES
    client.cpp
)

add_executable(CommClientTestClient ${WPEFRAMEWORK_TEST_COMMCLIENT_CLIENT_SOURCES})
target_link_libraries(CommClientTestClient ${WPEFRAMEWORK_TEST_COMMCLIENT_LIBS})

install(TARGETS CommClientTestClient DESTINATION bin)

