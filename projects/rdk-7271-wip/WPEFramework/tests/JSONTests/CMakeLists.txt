include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )
include_directories( ${WPEFRAMEWORK_DEVICES_INCLUDE_DIR} )

message(STATUS "\n **The JSONTests test have now been implemented as unit tests, and will beremoved from 'tests'. Please use the tests from 'unittests' by defining WPEFRAMEWORK_UNIT_TEST** \n")
set (WPEFRAMEWORK_TEST_JSONTESTS_LIBS
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
       devices
)

set(WPEFRAMEWORK_TEST_JSONTESTS_SOURCES
    main.cpp)

add_executable(json_tests ${WPEFRAMEWORK_TEST_JSONTESTS_SOURCES})
target_link_libraries(json_tests ${WPEFRAMEWORK_TEST_JSONTESTS_LIBS})

install(TARGETS json_tests DESTINATION bin)
