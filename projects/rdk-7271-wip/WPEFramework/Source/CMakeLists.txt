include(cmake/config.cmake)

include(config)
include(dump_vars)
include(setup_default_options)
include(platform)

include_directories(${WPEFRAMEWORK_ROOT}/Source)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

set(BUILD_SHARED_LIBS ON)
set(BUILD_TESTS ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   ON)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CONFIG_DIR "debug" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CONFIG_DIR "release" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    set(CONFIG_DIR "releaseMinSize" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(CONFIG_DIR "releaseMinSize" CACHE STRING "Build config directory" FORCE)
else()
    message(FATAL_ERROR "Invalid build type: " ${CMAKE_BUILD_TYPE})
endif()

set(BUILD_BASE_DIR ${WPEFRAMEWORK_ROOT}/Source/build CACHE STRING
        "Build root directory.")
set(CMAKE_BINARY_DIR ${BUILD_BASE_DIR}/${CONFIG_DIR} CACHE STRING
        "Build directory.")
message(STATUS "Build base directory: " ${BUILD_BASE_DIR})
message(STATUS "Build config directory: " ${CMAKE_BINARY_DIR})

set(OUTPUT_BASE_DIR ${WPEFRAMEWORK_ROOT}/Source/output)
message(STATUS "Output base directory: " ${OUTPUT_BASE_DIR})

add_definitions (-Wall -fPIC -std=c++11)
set(WPEFRAMEWORK_PLATFORM "PC_UNIX" CACHE STRING "Defines on what platform WPEFramework will run")

# TODO: Yocto doesn't like this, and seems to be unidiomatic CMake to set all of these flags ourselves.
if(WIN_MSVC)
    set(WPEFRAMEWORK_PLATFORM "PC_WINDOWS" CACHE STRING "Defines on what platform WPEFramework will run")
    set(WPEFRAMEWORK_WCHAR_SUPPORT ON CACHE STRING "Enable wide character support")
    set(FLAGS_CXX
            /Wall /WX- /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
    set(FLAGS_CXX_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
    set(FLAGS_CXX_RELEASE /Ox /GL /GS /Gy /Oi /MD)
    set(FLAGS_CXX_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
    set(FLAGS_CXX_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

    set(FLAGS_C
            /Wall /WX- /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
    set(FLAGS_C_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
    set(FLAGS_C_RELEASE /Ox /GL /GS /Gy /Oi /MD)
    set(FLAGS_C_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
    set(FLAGS_C_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

    set(DEFINES __DYNAMIC_LINKAGE__ _CRT_SECURE_NO_WARNINGS WIN32 _WINDOWS)
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(MINGW)
    set(WPEFRAMEWORK_PLATFORM "PC_MINGW" CACHE STRING "Defines on what platform WPEFramework will run")
    set(WPEFRAMEWORK_WCHAR_SUPPORT ON CACHE STRING "Enable wide character support")
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

#elseif(LINUX)
#    set(WPEFRAMEWORK_PLATFORM "PC_UNIX" CACHE STRING "Defines on what platform WPEFramework will run")
#    set(WPEFRAMEWORK_WCHAR_SUPPORT OFF CACHE STRING "Enable wide character support")
#    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC)
#    set(FLAGS_CXX_DEBUG -O0 -g)
#    set(FLAGS_CXX_RELEASE -O3)
#    set(FLAGS_CXX_MINSIZEREL -O3)
#    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

#    set(FLAGS_C -Wall -Wextra -fPIC)
#    set(FLAGS_C_DEBUG -O0 -g)
#    set(FLAGS_C_RELEASE -O3)
#    set(FLAGS_C_MINSIZEREL -O3)
#    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

#    set(DEFINES )
#    set(DEFINES_DEBUG _DEBUG)
#    set(DEFINES_RELEASE NDEBUG)
#    set(DEFINES_MINSIZEREL NDEBUG)
#    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(APPLE)
    set(WPEFRAMEWORK_PLATFORM "PC_MAC" CACHE STRING "Defines on what platform WPEFramework will run")
    set(WPEFRAMEWORK_WCHAR_SUPPORT OFF CACHE STRING "Enable wide character support")
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

#else()
#    message(FATAL_ERROR "Unsupported platform " ${CMAKE_HOST_SYSTEM})
endif()

list(APPEND DEFINES PLATFORM_${WPEFRAMEWORK_PLATFORM}=1)

if (BUILDREF_WPEFRAMEWORK)
    list(APPEND DEFINES BUILDREF_WPEFRAMEWORK="${WPEFRAMEWORK_VERSION}|${BUILDREF_WPEFRAMEWORK}")
else()
    list(APPEND DEFINES BUILDREF_WPEFRAMEWORK="${WPEFRAMEWORK_VERSION}")
endif()

if(CMAKE_USE_PTHREADS_INIT)
    message(STATUS "Adding pthread flags")
    list(APPEND FLAGS_CXX -pthread)
    list(APPEND FLAGS_C -pthread)
endif()

set(CMAKE_COMPILER_IS_CLANG OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Using Clang")
    set(CMAKE_COMPILER_IS_CLANG ON)
elseif(${CMAKE_COMPILER_IS_GNUCXX})
    message(STATUS "Using Gcc")
else()
    message(STATUS "Compiler " ${CMAKE_CXX_COMPILER_ID})
endif()

option(WPEFRAMEWORK_LIBWPEFRAMEWORK
       "Include the libWPEFramework library." ON)
option(WPEFRAMEWORK_CRYPTALGO
       "Include the encyption algorithm library." ON)
option(WPEFRAMEWORK_WEBSOCKET
       "Include the websocket library." OFF)
option(WPEFRAMEWORK_PROTOCOLS
       "Include the protocols library." ON)
option(WPEFRAMEWORK_TRACING
       "Include the tracing library." ON)
option(WPEFRAMEWORK_PROFILER
       "Include the profiler library." OFF)
option(WPEFRAMEWORK_RPC
       "Include the rpc library." OFF)
option(WPEFRAMEWORK_PROCESS
       "Include WPEProcess executable." ON)
option(WPEFRAMEWORK_PROXY_STUB
       "Include proxy/stubs." ON)
option(WPEFRAMEWORK_PLUGINS_LIB
       "Include plugins library." ON)
option(WPEFRAMEWORK_INTERFACES_LIB
       "Include interfaces library." ON)
option(WPEFRAMEWORK_WPEFRAMEWORK_EXECUTABLE
       "Include WPEFramework executable." ON)
option(WPEFRAMEWORK_VIRTUALINPUT
       "Include VirtualInput for external applications." OFF)
option(WPEFRAMEWORK_DEADLOCK_DETECTION
       "Enable deadlock detection tooling." OFF)
option(WPEFRAMEWORK_WCHAR_SUPPORT
       "Enable support for WCHAR in ccpsdk." OFF)
option(WPEFRAMEWORK_TEST_APPS
       "Include test applications." OFF)
option(WPEFRAMEWORK_UNIT_TEST
       "Include unit tests (new gtest based)." OFF)
option(WPEFRAMEWORK_TEST_MEASUREMENTS
       "Include code coverage measurments." OFF)
option(WPEFRAMEWORK_STUB_HARDWARE
       "Use hardware stubs for unittests)." OFF)
option(INSTALL_HEADERS_TO_TARGET
       "Install WPEFramework headers." OFF)

if(NOT WPEFRAMEWORK_WCHAR_SUPPORT)
    list(APPEND DEFINES __NO_WCHAR_SUPPORT__)
endif()

if(WPEFRAMEWORK_DEADLOCK_DETECTION)
    list(APPEND DEFINES CRITICAL_SECTION_LOCK_LOG)
endif()

if(WPEFRAMEWORK_STUB_HARDWARE)
    list(APPEND DEFINES STUB_HARDWARE)
endif()

if(WPEFRAMEWORK_TEST_MEASUREMENTS)
    list(APPEND CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS -lgcov)
endif ()

if(${WPEFRAMEWORK_PLATFORM} STREQUAL "INTELCE")
    find_package(IntelOSAL REQUIRED)
    find_package(IntelGDL REQUIRED)
endif()

if(WPEFRAMEWORK_LIBWPEFRAMEWORK)
    set(WPEFRAMEWORK_LIBWPEFRAMEWORK_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/core")
    add_subdirectory(core)
endif()

if(WPEFRAMEWORK_CRYPTALGO)
    set(WPEFRAMEWORK_CRYPTALGO_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/cryptalgo")
    add_subdirectory(cryptalgo)
endif()

if(WPEFRAMEWORK_VIRTUALINPUT)
    set(WPEFRAMEWORK_VIRTUALINPUT_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/virtualinput")
    add_subdirectory(virtualinput)
endif()


if(WPEFRAMEWORK_PROVISIONPROXY)
    set(WPEFRAMEWORK_PROVISIONPROXY_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/provisionproxy")
    add_subdirectory(provisionproxy)
endif()

if(WPEFRAMEWORK_WEBSOCKET)
    find_package(ZLIB REQUIRED)
    set(WPEFRAMEWORK_WEBSOCKET_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/websocket")
    add_subdirectory(websocket)
endif()

if(WPEFRAMEWORK_TRACING)
    set(WPEFRAMEWORK_TRACING_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/tracing")
    add_subdirectory(tracing)
endif()

if(WPEFRAMEWORK_PROFILER)
    set(WPEFRAMEWORK_PROFILER_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/profiler")
    add_subdirectory(profiler)
endif()

if(WPEFRAMEWORK_RPC)
    set(WPEFRAMEWORK_RPC_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/rpc")
    add_subdirectory(rpc)
endif()

if(WPEFRAMEWORK_PROTOCOLS)
    set(WPEFRAMEWORK_RPC_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/protocols")
    add_subdirectory(protocols)
endif()

if(WPEFRAMEWORK_PROCESS)
    set(WPEFRAMEWORK_PROCESS_INCLUDE_DIR "${WPEFRAMEWORK_ROOT}/Source/WPEProcess")
    add_subdirectory(WPEProcess)
endif()

if(WPEFRAMEWORK_UNIT_TEST)
    add_subdirectory(unittests)
endif()

map()
    key(plugins)
    key(tracing)
    key(version)
    val(${WPEFRAMEWORK_VERSION_MAJOR}.${WPEFRAMEWORK_VERSION_MINOR}.${WPEFRAMEWORK_VERSION_REVISION})
end()
# get the result
ans(WPEFRAMEWORK_CONFIG)

map_append(${WPEFRAMEWORK_CONFIG} tracing ___array___)
map_append(${WPEFRAMEWORK_CONFIG} plugins ___array___)
map_append(${WPEFRAMEWORK_CONFIG} plugins ${WPEFRAMEWORK_PLUGIN_CONTROLLER})

if(WPEFRAMEWORK_PROXY_STUB)
    add_subdirectory(proxystubs)
endif(WPEFRAMEWORK_PROXY_STUB)

if(WPEFRAMEWORK_PLUGINS_LIB)
    add_subdirectory(plugins)
endif(WPEFRAMEWORK_PLUGINS_LIB)

if(WPEFRAMEWORK_INTERFACES_LIB)
    add_subdirectory(interfaces)
endif(WPEFRAMEWORK_INTERFACES_LIB)

if(WPEFRAMEWORK_WPEFRAMEWORK_EXECUTABLE)
    add_subdirectory(WPEFramework)
endif(WPEFRAMEWORK_WPEFRAMEWORK_EXECUTABLE)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/WPEFramework.pc.in" 
    "${CMAKE_BINARY_DIR}/WPEFramework.pc"
    @ONLY)

# uninstall target
configure_file(
    ${WPEFRAMEWORK_ROOT}/Source/cmake/uninstall.cmake.in
    ${CMAKE_BINARY_DIR}/uninstall.cmake
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/uninstall.cmake)

add_custom_target(install_${PROJECT_NAME}
    $(MAKE) install
    COMMENT "Installing ${PROJECT_NAME}")

install(FILES ${CMAKE_BINARY_DIR}/WPEFramework.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")

install(DIRECTORY cmake/config DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake)
install(FILES cmake/config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/cmake)

#setup_default_options()

json_write("${CMAKE_CURRENT_LIST_DIR}/Config.json" ${WPEFRAMEWORK_CONFIG})
install(FILES Config.json DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/WPEFramework/ RENAME config.json)

