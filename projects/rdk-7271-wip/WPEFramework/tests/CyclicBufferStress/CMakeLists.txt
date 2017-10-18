include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_CYCLICBUFFERSTRESS_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       WPEFrameworkCore
)

set (WPEFRAMEWORK_TEST_CYCLICBUFFERSTRESS_SOURCES
    main.cpp
    client.cpp
    server.cpp
    utils.cpp
)

add_definitions(-Wall -std=c++11)
add_executable(CyclicBufferStress ${WPEFRAMEWORK_TEST_CYCLICBUFFERSTRESS_SOURCES})

if(WPEFRAMEWORK_TEST_MEASUREMENTS)
    list(APPEND CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS -lgcov)
endif ()

target_link_libraries(CyclicBufferStress ${WPEFRAMEWORK_TEST_CYCLICBUFFERSTRESS_LIBS})

install(TARGETS CyclicBufferStress DESTINATION bin)

