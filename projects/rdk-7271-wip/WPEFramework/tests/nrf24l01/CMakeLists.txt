include_directories( ${WPEFRAMEWORK_GENERICS_INCLUDE_DIR} )
include_directories( ${WPEFRAMEWORK_DEVICES_INCLUDE_DIR} )

set (WPEFRAMEWORK_TEST_NRF24L01_LIBS 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_DL_LIBS}
       generics
       devices
)

set (WPEFRAMEWORK_TEST_NRF24L01_SOURCES
       nrf24l01.cpp
    )

add_executable(nrf24l01 ${WPEFRAMEWORK_TEST_NRF24L01_SOURCES})
target_link_libraries(nrf24l01 ${WPEFRAMEWORK_TEST_NRF24L01_LIBS})

install(TARGETS nrf24l01 DESTINATION bin)
