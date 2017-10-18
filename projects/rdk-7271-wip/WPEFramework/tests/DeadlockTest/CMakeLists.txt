include_directories(
    ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR}
)

set (WPEFRAMEWORK_TEST_DEADLOCK_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
)

set (WPEFRAMEWORK_TEST_DEADLOCK_SOURCES
    deadlockTest.cpp
)

add_executable(deadlockTest ${WPEFRAMEWORK_TEST_DEADLOCK_SOURCES})
target_link_libraries(deadlockTest ${WPEFRAMEWORK_TEST_DEADLOCK_LIBS})

install(TARGETS deadlockTest DESTINATION bin)
