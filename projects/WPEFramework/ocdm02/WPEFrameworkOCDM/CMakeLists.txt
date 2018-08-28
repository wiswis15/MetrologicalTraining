cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_SYSROOT}/${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake"
    ${CMAKE_MODULE_PATH})

if(CMAKE_CROSSCOMPILING)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake")
    foreach(searchPath ${CMAKE_FIND_ROOT_PATH})
        if(EXISTS "${searchPath}/usr/include/WPEFramework/cmake/config.cmake")
            include(${searchPath}/usr/include/WPEFramework/cmake/config.cmake)
        endif(EXISTS "${searchPath}/usr/include/WPEFramework/cmake/config.cmake")
    endforeach()
else(CMAKE_CROSSCOMPILING)
    if(EXISTS "${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake/config.cmake")
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake")
        include(${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake/config.cmake)
    else()
        if(EXISTS "${WPEFRAMEWORK_ROOT}/Source/cmake/config.cmake")
            list(APPEND CMAKE_MODULE_PATH "${WPEFRAMEWORK_ROOT}/Source/cmake")
            include(${WPEFRAMEWORK_ROOT}/Source/cmake/config.cmake)
        endif()
    endif()
endif(CMAKE_CROSSCOMPILING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

find_package(WPEFramework)

include_directories("${CMAKE_INSTALL_PREFIX}/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}"
                    "${WPEFRAMEWORK_INCLUDE_DIRS}"
                    "${WPEFRAMEWORK_INCLUDE_DIRS}/ocdm") # TODO: Should we explicitly mention ocdm here?

add_library(WPEFrameworkOCDM SHARED
   AccessorOCDM.cpp
   ProxyStubs.cpp
   DataExchange.cpp
)

target_link_libraries(WPEFrameworkOCDM WPEFrameworkCore WPEFrameworkProtocols)

file (GLOB HEADER_FILE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# TODO: should this overwrite "${CMAKE_INSTALL_PREFIX}/include/wpeframework/ocdm"?  
install (FILES ${HEADER_FILE_LIST} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WPEFramework/ocdm-accessor")

install(TARGETS WPEFrameworkOCDM DESTINATION lib)

