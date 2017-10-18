include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_IPV6_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
)

set (WPEFRAMEWORK_TEST_IPV6_SOURCES
       ipv6-test.cpp
    )

add_executable(ipv6-test ${WPEFRAMEWORK_TEST_IPV6_SOURCES})
target_link_libraries(ipv6-test ${WPEFRAMEWORK_TEST_IPV6_LIBS} )

install(TARGETS ipv6-test DESTINATION bin)
