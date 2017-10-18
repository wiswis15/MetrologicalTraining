include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_SERVERCLIENT_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
)

set (WPEFRAMEWORK_TEST_SERVERCLIENT_SOURCES
       timeSetTest.cpp
    )

add_executable(timeSetTest ${WPEFRAMEWORK_TEST_SERVERCLIENT_SOURCES})
target_link_libraries(timeSetTest ${WPEFRAMEWORK_TEST_SERVERCLIENT_LIBS} )

install(TARGETS timeSetTest DESTINATION bin)
