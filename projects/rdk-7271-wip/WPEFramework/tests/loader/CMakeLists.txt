#include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )

set(WPEFRAMEWORK_TEST_LOADER_LIBS 
   ${CMAKE_THREAD_LIBS_INIT}
   ${CMAKE_DL_LIBS}
   WPEFrameworkCore
)

set(WPEFRAMEWORK_TEST_LOADER_SOURCES
    main.cpp
)

add_definitions (-Wall -std=c++11)
add_executable(TestLoader ${WPEFRAMEWORK_TEST_LOADER_SOURCES})
target_link_libraries(TestLoader ${WPEFRAMEWORK_TEST_LOADER_LIBS})

install(TARGETS TestLoader DESTINATION bin)

