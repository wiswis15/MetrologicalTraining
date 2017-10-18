include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_GETID_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
)

set (WPEFRAMEWORK_TEST_GETID_SOURCES
       getid.cpp
    )

add_executable(getid ${WPEFRAMEWORK_TEST_GETID_SOURCES})
target_link_libraries(getid ${WPEFRAMEWORK_TEST_GETID_LIBS} )

install(TARGETS getid DESTINATION bin)
