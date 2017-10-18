include_directories(
    ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR}
    ${WPEFRAMEWORK_MQC_INCLUDE_DIR}
)

set (WPEFRAMEWORK_TEST_MQC_LIBS 
   ${CMAKE_THREAD_LIBS_INIT}
   ${CMAKE_DL_LIBS}
   generics
   mqc
    rt
)

set (WPEFRAMEWORK_TEST_MQC_SOURCES
    InitMessage.cpp
    HandleInit.cpp
    AsyncMessage.cpp
    HandleAsync.cpp
    MonitorMessages.cpp
    ProxyMessages.cpp
    ProxyHandlers.cpp
    mqcTest.cpp
    LaunchTest.cpp
    MonitorHandlers.cpp
)

add_executable(mqcTest ${WPEFRAMEWORK_TEST_MQC_SOURCES})
target_link_libraries(mqcTest ${WPEFRAMEWORK_TEST_MQC_LIBS})

install(TARGETS mqcTest DESTINATION bin)

