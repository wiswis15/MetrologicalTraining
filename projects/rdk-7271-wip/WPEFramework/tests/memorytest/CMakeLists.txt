include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )
include_directories( ${WPEFRAMEWORK_DEVICES_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_MEMORYTEST_LIBS
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
       devices
)

set(WPEFRAMEWORK_TEST_MEMORYTEST_SOURCES
    main.cpp)

add_executable(memory_test ${WPEFRAMEWORK_TEST_MEMORYTEST_SOURCES})
target_link_libraries(memory_test ${WPEFRAMEWORK_TEST_MEMORYTEST_LIBS})

install(TARGETS memory_test DESTINATION bin)
