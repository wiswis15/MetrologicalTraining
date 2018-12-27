add_executable(framework01_test_core
   test_add01.cpp
   test_vector01.cpp
   test_bool01.cpp
   test_string01.cpp
   test_fixture01.cpp
)

target_link_libraries(framework01_test_core 
    ${GTEST_LIBRARY}
    ${GTEST_MAIN_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    FrameworkCore
)
