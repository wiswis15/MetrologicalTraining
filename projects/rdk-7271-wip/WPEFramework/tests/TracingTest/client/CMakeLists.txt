set (WPEFRAMEWORK_TEST_TRACINGTEST_CLIENT_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       WPEFrameworkCore
       WPEFrameworkTracing
)

set (WPEFRAMEWORK_TEST_TRACINGTEST_CLIENT_SOURCES
    Module.cpp
    main.cpp
    ../utils.cpp
)

add_definitions(-Wall -std=c++11)
add_executable(TracingTestClient ${WPEFRAMEWORK_TEST_TRACINGTEST_CLIENT_SOURCES})

target_link_libraries(TracingTestClient ${WPEFRAMEWORK_TEST_TRACINGTEST_CLIENT_LIBS})

install(TARGETS TracingTestClient DESTINATION bin)
