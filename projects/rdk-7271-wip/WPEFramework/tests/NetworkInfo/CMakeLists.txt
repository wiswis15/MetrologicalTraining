include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_NETWORKINFO_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
)

set (WPEFRAMEWORK_TEST_NETWORKINFO_SOURCES
    main.cpp
)

add_executable(NetworkInfo ${WPEFRAMEWORK_TEST_NETWORKINFO_SOURCES})
target_link_libraries(NetworkInfo ${WPEFRAMEWORK_TEST_NETWORKINFO_LIBS})

install(TARGETS NetworkInfo DESTINATION bin)
