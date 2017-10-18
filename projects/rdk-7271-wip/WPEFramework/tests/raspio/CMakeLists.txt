include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )
include_directories( ${WPEFRAMEWORK_DEVICES_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_RASPIO_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
       devices
)

set (WPEFRAMEWORK_TEST_RASPIO_SOURCES
       raspio.cpp
    )

add_executable(raspio ${WPEFRAMEWORK_TEST_RASPIO_SOURCES})
target_link_libraries(raspio ${WPEFRAMEWORK_TEST_RASPIO_LIBS})

install(TARGETS raspio DESTINATION bin)
