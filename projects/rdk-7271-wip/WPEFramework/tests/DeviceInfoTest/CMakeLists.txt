include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_DEVICEINFO_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
       tracing
)

set (WPEFRAMEWORK_TEST_DEVICEINFO_SOURCES
       getdeviceinfo.cpp
    )

add_executable(getdeviceinfo ${WPEFRAMEWORK_TEST_DEVICEINFO_SOURCES})
target_link_libraries(getdeviceinfo ${WPEFRAMEWORK_TEST_DEVICEINFO_LIBS} )

install(TARGETS getdeviceinfo DESTINATION bin)
