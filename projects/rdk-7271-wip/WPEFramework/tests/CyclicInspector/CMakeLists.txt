set (WPEFRAMEWORK_TEST_CYCLICINSPECTOR_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       WPEFrameworkCore
)

set (WPEFRAMEWORK_TEST_CYCLICINSPECTOR_SOURCES
    main.cpp
)

add_definitions(-Wall -std=c++11)
add_executable(CyclicInspector ${WPEFRAMEWORK_TEST_CYCLICINSPECTOR_SOURCES})

target_link_libraries(CyclicInspector ${WPEFRAMEWORK_TEST_CYCLICINSPECTOR_LIBS})

install(TARGETS CyclicInspector DESTINATION bin)

