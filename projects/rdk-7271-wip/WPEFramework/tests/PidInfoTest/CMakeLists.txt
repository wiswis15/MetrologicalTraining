include_directories(
    ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR}
)

set (WPEFRAMEWORK_PIDINFOTEST_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
)

set (WPEFRAMEWORK_PIDINFOTEST_SOURCES
    pidInfoTest.cpp
)

add_executable(pidInfoTest ${WPEFRAMEWORK_PIDINFOTEST_SOURCES})
target_link_libraries(pidInfoTest ${WPEFRAMEWORK_PIDINFOTEST_LIBS})

install(TARGETS pidInfoTest DESTINATION bin)

