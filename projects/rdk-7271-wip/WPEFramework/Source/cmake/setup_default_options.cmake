include(${WPEFRAMEWORK_ROOT}/Source/cmake/list_to_string.cmake)
include(${WPEFRAMEWORK_ROOT}/Source/cmake/add_defines.cmake)

# Sets up the default compiler options, based on a number of defined variables:
# FLAGS_CXX                 Compiler flags for all platforms, for langurage C++
# FLAGS_CXX_DEBUG           Compiler flags for debug build, for langurage C++
# FLAGS_CXX_RELEASE         Compiler flags for release build, for langurage C++
# FLAGS_CXX_MINSIZEREL      Compiler flags for release min size build, for langurage C++
# FLAGS_CXX_RELWITHDEBINFO  Compiler flags for release with debug info build, for langurage C++
# FLAGS_C                   Compiler flags for all platforms, for langurage C
# FLAGS_C_DEBUG             Compiler flags for debug build, for langurage C
# FLAGS_C_RELEASE           Compiler flags for release build, for langurage C
# FLAGS_C_MINSIZEREL        Compiler flags for release min size build, for langurage C
# FLAGS_C_RELWITHDEBINFO    Compiler flags for release with debug info build, for langurage C
# DEFINES                   Compiler definitions for all platforms, for langurage C and C++
# DEFINES_DEBUG             Compiler definitions for debug build, for langurage C and C++
# DEFINES_RELEASE           Compiler definitions for release build, for langurage C and C++
# DEFINES_MINSIZEREL        Compiler definitions for release min size build, for langurage C and C++
# DEFINES_RELWITHDEBINFO    Compiler definitions for release with debug info build, for langurage C and C++
#
# The following standard variables are set and placed in the cache (by appending the contents of the mentioned
# lists, and subsequently converting the lists to space separated string):
#
# CMAKE_CXX_FLAGS                   FLAGS_CXX DEFINES
# CMAKE_CXX_FLAGS_DEBUG             FLAGS_CXX FLAGS_CXX_DEBUG DEFINES DEFINES_DEBUG
# CMAKE_CXX_FLAGS_RELEASE           FLAGS_CXX FLAGS_CXX_RELEASE DEFINES DEFINES_RELEASE
# CMAKE_CXX_FLAGS_MINSIZEREL        FLAGS_CXX FLAGS_CXX_MINSIZEREL DEFINES DEFINES_MINSIZEREL
# CMAKE_CXX_FLAGS_RELWITHDEBINFO    FLAGS_CXX FLAGS_CXX_RELWITHDEBINFO DEFINES DEFINES_RELWITHDEBINFO
# CMAKE_C_FLAGS                     FLAGS_C DEFINES
# CMAKE_C_FLAGS_DEBUG               FLAGS_C FLAGS_C_DEBUG DEFINES DEFINES_DEBUG
# CMAKE_C_FLAGS_RELEASE             FLAGS_C FLAGS_C_RELEASE DEFINES DEFINES_RELEASE
# CMAKE_C_FLAGS_MINSIZEREL          FLAGS_C FLAGS_C_MINSIZEREL DEFINES DEFINES_MINSIZEREL
# CMAKE_C_FLAGS_RELWITHDEBINFO      FLAGS_C FLAGS_C_RELWITHDEBINFO DEFINES DEFINES_RELWITHDEBINFO

function(setup_default_options)
    set(OPTIONS_CXX ${FLAGS_CXX})
    set(OPTIONS_CXX_DEBUG ${FLAGS_CXX_DEBUG})
    set(OPTIONS_CXX_RELEASE ${FLAGS_CXX_RELEASE})
    set(OPTIONS_CXX_MINSIZEREL ${FLAGS_CXX_MINSIZEREL})
    set(OPTIONS_CXX_RELWITHDEBINFO ${FLAGS_CXX_RELWITHDEBINFO})

    set(OPTIONS_C ${FLAGS_C})
    set(OPTIONS_C_DEBUG ${FLAGS_C_DEBUG})
    set(OPTIONS_C_RELEASE ${FLAGS_C_RELEASE})
    set(OPTIONS_C_MINSIZEREL ${FLAGS_C_MINSIZEREL})
    set(OPTIONS_C_RELWITHDEBINFO ${FLAGS_C_RELWITHDEBINFO})

    add_defines(OPTIONS_CXX DEFINES)
    add_defines(OPTIONS_CXX_DEBUG DEFINES_DEBUG)
    add_defines(OPTIONS_CXX_RELEASE DEFINES_RELEASE)
    add_defines(OPTIONS_CXX_MINSIZEREL DEFINES_MINSIZEREL)
    add_defines(OPTIONS_CXX_RELWITHDEBINFO DEFINES_RELWITHDEBINFO)

    add_defines(OPTIONS_C DEFINES)
    add_defines(OPTIONS_C_DEBUG DEFINES_DEBUG)
    add_defines(OPTIONS_C_RELEASE DEFINES_RELEASE)
    add_defines(OPTIONS_C_MINSIZEREL DEFINES_MINSIZEREL)
    add_defines(OPTIONS_C_RELWITHDEBINFO DEFINES_RELWITHDEBINFO)

    list_to_string(OPTIONS_CXX OPTIONS_CXX_STRING)
    list_to_string(OPTIONS_CXX_DEBUG OPTIONS_CXX_DEBUG_STRING)
    list_to_string(OPTIONS_CXX_RELEASE OPTIONS_CXX_RELEASE_STRING)
    list_to_string(OPTIONS_CXX_MINSIZEREL OPTIONS_CXX_MINSIZEREL_STRING)
    list_to_string(OPTIONS_CXX_RELWITHDEBINFO OPTIONS_CXX_RELWITHDEBINFO_STRING)

    list_to_string(OPTIONS_C OPTIONS_C_STRING)
    list_to_string(OPTIONS_C_DEBUG OPTIONS_C_DEBUG_STRING)
    list_to_string(OPTIONS_C_RELEASE OPTIONS_C_RELEASE_STRING)
    list_to_string(OPTIONS_C_MINSIZEREL OPTIONS_C_MINSIZEREL_STRING)
    list_to_string(OPTIONS_C_RELWITHDEBINFO OPTIONS_C_RELWITHDEBINFO_STRING)

#    message(STATUS "C++ Options:                        " ${OPTIONS_CXX_STRING})
#    message(STATUS "C++ Options - debug:                " ${OPTIONS_CXX_DEBUG_STRING})
#    message(STATUS "C++ Options - release:              " ${OPTIONS_CXX_RELEASE_STRING})
#    message(STATUS "C++ Options - release min size:     " ${OPTIONS_CXX_MINSIZEREL_STRING})
#    message(STATUS "C++ Options - release debug info:   " ${OPTIONS_CXX_RELWITHDEBINFO_STRING})
#
#    message(STATUS "C Options:                          " ${OPTIONS_C_STRING})
#    message(STATUS "C Options - debug:                  " ${OPTIONS_C_DEBUG_STRING})
#    message(STATUS "C Options - release:                " ${OPTIONS_C_RELEASE_STRING})
#    message(STATUS "C Options - release min size:       " ${OPTIONS_C_MINSIZEREL_STRING})
#    message(STATUS "C Options - release debug info:     " ${OPTIONS_C_RELWITHDEBINFO_STRING})

    set(CMAKE_CXX_FLAGS ${OPTIONS_CXX_STRING} 
        CACHE STRING
        "Standard compiler options" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG ${OPTIONS_CXX_DEBUG_STRING}
        CACHE STRING
        "Flags used by the compiler during debug builds" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE ${OPTIONS_CXX_RELEASE_STRING}
        CACHE STRING
        "Flags used by the compiler during release builds." FORCE)
    set(CMAKE_CXX_FLAGS_MINSIZEREL ${OPTIONS_CXX_MINSIZEREL_STRING}
        CACHE STRING
        "Flags used by the compiler during release builds for minimum size." FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${OPTIONS_CXX_RELWITHDEBINFO_STRING}
        CACHE STRING
        "Flags used by the compiler during release builds with debug info." FORCE)

    set(CMAKE_C_FLAGS ${OPTIONS_C_STRING} 
        CACHE STRING
        "Standard compiler options" FORCE)
    set(CMAKE_C_FLAGS_DEBUG ${OPTIONS_C_DEBUG_STRING}
        CACHE STRING
        "Flags used by the compiler during debug builds" FORCE)
    set(CMAKE_C_FLAGS_RELEASE ${OPTIONS_C_RELEASE_STRING}
        CACHE STRING
        "Flags used by the compiler during release builds." FORCE)
    set(CMAKE_C_FLAGS_MINSIZEREL ${OPTIONS_C_MINSIZEREL_STRING}
        CACHE STRING
        "Flags used by the compiler during release builds for minimum size." FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO ${OPTIONS_C_RELWITHDEBINFO_STRING}
        CACHE STRING
        "Flags used by the compiler during release builds with debug info." FORCE)

endfunction()
