set (WPEFRAMEWORK_TEST_TRACINGTEST_SERVER_LIBS 
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    WPEFrameworkCore
    WPEFrameworkTracing
)

set (WPEFRAMEWORK_TEST_TRACINGTEST_SERVER_SOURCES
    Module.cpp
    main.cpp
    ../utils.cpp
)

add_definitions(-Wall -std=c++11)
add_executable(TracingTestServer ${WPEFRAMEWORK_TEST_TRACINGTEST_SERVER_SOURCES})

target_link_libraries(TracingTestServer ${WPEFRAMEWORK_TEST_TRACINGTEST_SERVER_LIBS})

install(TARGETS TracingTestServer DESTINATION bin)
