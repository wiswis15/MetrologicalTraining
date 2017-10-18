function(dump_vars)
    # ------------------------- Begin Generic CMake Variable Logging ------------------

    MESSAGE( STATUS "Variable dump" )

    MESSAGE( STATUS "CMake variables" )

    # version number for CMake
    MESSAGE( STATUS "CMAKE version:                             " ${CMAKE_MAJOR_VERSION} "." ${CMAKE_MINOR_VERSION} "." ${CMAKE_PATCH_VERSION} "." ${CMAKE_TWEAK_VERSION} )

    # The version number combined, eg. 2.8.4.20110222-ged5ba for a Nightly build. or 2.8.4 for a Release build.
    MESSAGE( STATUS "CMAKE_VERSION:                             " ${CMAKE_VERSION} )

    # if you are building in-source, this is the same as WPEFRAMEWORK_ROOT, otherwise
    # this is the top level directory of your build tree 
    MESSAGE( STATUS "CMAKE_BINARY_DIR:                          " ${CMAKE_BINARY_DIR} )

    # this is the complete path of the cmake which runs currently (e.g. /usr/local/bin/cmake) 
    MESSAGE( STATUS "CMAKE_COMMAND:                             " ${CMAKE_COMMAND} )

    # if you are building in-source, this is the same as CMAKE_CURRENT_SOURCE_DIR, otherwise this 
    # is the directory where the compiled or generated files from the current CMakeLists.txt will go to 
    MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR:                  " ${CMAKE_CURRENT_BINARY_DIR} )

    # this is the filename including the complete path of the file where this variable is used. 
    MESSAGE( STATUS "CMAKE_CURRENT_LIST_FILE:                   " ${CMAKE_CURRENT_LIST_FILE} )

    #  this is the directory of the listfile currently being processed. 
    MESSAGE( STATUS "CMAKE_CURRENT_LIST_DIR:                    " ${CMAKE_CURRENT_LIST_DIR} )

    # this is linenumber where the variable is used
    MESSAGE( STATUS "CMAKE_CURRENT_LIST_LINE:                   " ${CMAKE_CURRENT_LIST_LINE} )

    # this is the directory where the currently processed CMakeLists.txt is located in 
    MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR:                  " ${CMAKE_CURRENT_SOURCE_DIR} )

    # this is the directory, from which cmake was started, i.e. the top level source directory 
    MESSAGE( STATUS "CMAKE_SOURCE_DIR:                          " ${CMAKE_SOURCE_DIR} )

    # the directory within the current binary directory that contains all the CMake generated files. 
    # Typically evaluates to "/CMakeFiles". Note the leading slash for the directory. Typically used 
    # with the current binary directory, i.e. ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY} 
    MESSAGE( STATUS "CMAKE_FILES_DIRECTORY:                     " ${CMAKE_FILES_DIRECTORY} )

    # tell CMake to search first in directories listed in CMAKE_MODULE_PATH
    # when you use FIND_PACKAGE() or INCLUDE()
    MESSAGE( STATUS "CMAKE_MODULE_PATH:                         " ${CMAKE_MODULE_PATH} )

    # this is the CMake installation directory 
    MESSAGE( STATUS "CMAKE_ROOT:                                " ${CMAKE_ROOT} )

    # set this variable to specify a common place where CMake should put all executable files
    # (instead of CMAKE_CURRENT_BINARY_DIR)
    MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH:                    " ${EXECUTABLE_OUTPUT_PATH} )

    # set this variable to specify a common place where CMake should put all libraries 
    # (instead of CMAKE_CURRENT_BINARY_DIR)
    MESSAGE( STATUS "LIBRARY_OUTPUT_PATH:                       " ${LIBRARY_OUTPUT_PATH} )

    MESSAGE( STATUS "Project variables" )

    # the name of the project set by PROJECT() command. 
    MESSAGE( STATUS "PROJECT_NAME:                              " ${PROJECT_NAME} )

    # the name of the first project set by the PROJECT() command, i.e. the top level project
    MESSAGE( STATUS "CMAKE_PROJECT_NAME :                       " ${CMAKE_PROJECT_NAME} )

    # contains the full path to the top level directory of your build tree 
    MESSAGE( STATUS "PROJECT_BINARY_DIR:                        " ${PROJECT_BINARY_DIR} )

    # contains the full path to the root of your project source directory,
    # i.e. to the nearest directory where CMakeLists.txt contains the PROJECT() command 
    MESSAGE( STATUS "PROJECT_SOURCE_DIR:                        " ${PROJECT_SOURCE_DIR} )

    # this is used when searching for include files e.g. using the FIND_PATH() command.
    MESSAGE( STATUS "CMAKE_INCLUDE_PATH:                        " ${CMAKE_INCLUDE_PATH} )

    # this is used when searching for libraries e.g. using the FIND_LIBRARY() command.
    MESSAGE( STATUS "CMAKE_LIBRARY_PATH:                        " ${CMAKE_LIBRARY_PATH} )

    # This is used when searching for include files, binaries, or libraries using either
    # the FIND_PACKAGE(), FIND_PATH(), FIND_PROGRAM(), or FIND_LIBRARY() commands. For
    # each path in the CMAKE_PREFIX_PATH list, CMake will check "PATH/include" and "PATH"
    # when FIND_PATH() is called, "PATH/bin" and "PATH" when FIND_PROGRAM() is called, and 
    # "PATH/lib" and "PATH" when FIND_LIBRARY() is called. See the documentation for
    # FIND_PACKAGE(), FIND_LIBRARY(), FIND_PATH(), and FIND_PROGRAM() for more details.
    MESSAGE( STATUS "CMAKE_PREFIX_PATH :                        " ${CMAKE_PREFIX_PATH} )

    # If set during installation CMake will install all files whether they have changed
    # or not. The default when this is not set is to install only files that have changed
    # since the previous installation. In both cases all files are reported to indicate
    # CMake knows they are up to date in the installed location.
    MESSAGE( STATUS "CMAKE_INSTALL_ALWAYS :                     " ${CMAKE_INSTALL_ALWAYS} )

    MESSAGE( STATUS "System variables" )

    # the complete system name, e.g. "Linux-2.4.22", "FreeBSD-5.4-RELEASE" or "Windows 5.1" 
    MESSAGE( STATUS "CMAKE_SYSTEM:                              " ${CMAKE_SYSTEM} )

    # the short system name, e.g. "Linux", "FreeBSD" or "Windows"
    MESSAGE( STATUS "CMAKE_SYSTEM_NAME:                         " ${CMAKE_SYSTEM_NAME} )

    # only the version part of CMAKE_SYSTEM 
    MESSAGE( STATUS "CMAKE_SYSTEM_VERSION:                      " ${CMAKE_SYSTEM_VERSION} )

    # the processor name (e.g. "Intel(R) Pentium(R) M processor 2.00GHz") 
    MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR:                    " ${CMAKE_SYSTEM_PROCESSOR} )

    MESSAGE( STATUS "CMake generators" )

    # the generator specified on the commandline.
    MESSAGE( STATUS "CMAKE_GENERATOR:                           " ${CMAKE_GENERATOR} )

    # is TRUE on all UNIX-like OS's, including Apple OS X and CygWin
    MESSAGE( STATUS "UNIX:                                      " ${UNIX} )

    # is TRUE on Windows, including CygWin 
    MESSAGE( STATUS "WIN32:                                     " ${WIN32} )

    # is TRUE on Apple OS X
    MESSAGE( STATUS "APPLE:                                     " ${APPLE} )

    # is TRUE when using the MinGW compiler in Windows
    MESSAGE( STATUS "MINGW:                                     " ${MINGW} )

    # is TRUE when using the MSYS developer environment in Windows
    MESSAGE( STATUS "MSYS:                                      " ${MSYS} )

    # is TRUE on Windows when using the CygWin version of cmake
    MESSAGE( STATUS "CYGWIN:                                    " ${CYGWIN} )

    # is TRUE on Windows when using a Borland compiler 
    MESSAGE( STATUS "BORLAND:                                   " ${BORLAND} )

    # is TRUE on Windows when using the Open Watcom compiler
    MESSAGE( STATUS "WATCOM:                                    " ${WATCOM} )

    # Microsoft compiler 
    MESSAGE( STATUS "MSVC:                                      " ${MSVC} )
    MESSAGE( STATUS "MSVC_IDE:                                  " ${MSVC_IDE} )
    MESSAGE( STATUS "MSVC60:                                    " ${MSVC60} )
    MESSAGE( STATUS "MSVC70 (VS2002):                           " ${MSVC70} )
    MESSAGE( STATUS "MSVC71 (VS2003):                           " ${MSVC71} )
    MESSAGE( STATUS "MSVC80 (VS2005):                           " ${MSVC80} )
    MESSAGE( STATUS "CMAKE_COMPILER_2005:                       " ${CMAKE_COMPILER_2005} )
    MESSAGE( STATUS "MSVC90 (VS2008):                           " ${MSVC90} )
    MESSAGE( STATUS "MSVC10 (VS2010):                           " ${MSVC10} )
    MESSAGE( STATUS "MSVC11 (VS2012):                           " ${MSVC11} )
    MESSAGE( STATUS "MSVC12 (VS2013):                           " ${MSVC12} )
    MESSAGE( STATUS "MSVC14 (VS2015):                           " ${MSVC14} )

    MESSAGE( STATUS "XCODE:                                     " ${XCODE} )

    MESSAGE( STATUS "CMake options" )

    # set this to true if you don't want to rebuild the object files if the rules have changed, 
    # but not the actual source files or headers (e.g. if you changed the some compiler switches) 
    MESSAGE( STATUS "CMAKE_SKIP_RULE_DEPENDENCY:                " ${CMAKE_SKIP_RULE_DEPENDENCY} )

    # since CMake 2.1 the install rule depends on all, i.e. everything will be built before installing. 
    # If you don't like this, set this one to true.
    MESSAGE( STATUS "CMAKE_SKIP_INSTALL_ALL_DEPENDENCY:         " ${CMAKE_SKIP_INSTALL_ALL_DEPENDENCY} )

    # If set, runtime paths are not added when using shared libraries. Default it is set to OFF
    MESSAGE( STATUS "CMAKE_SKIP_RPATH:                          " ${CMAKE_SKIP_RPATH} )

    # If set, runtime paths are not added when using shared libraries. Default it is set to OFF
    MESSAGE( STATUS "CMAKE_INCLUDE_CURRENT_DIR:                 " ${CMAKE_INCLUDE_CURRENT_DIR} )

    # If set, runtime paths are not added when using shared libraries. Default it is set to OFF
    MESSAGE( STATUS "CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE:  " ${CMAKE_SKIP_RPATH} )

    # set this to true if you are using makefiles and want to see the full compile and link 
    # commands instead of only the shortened ones 
    MESSAGE( STATUS "CMAKE_VERBOSE_MAKEFILE:                    " ${CMAKE_VERBOSE_MAKEFILE} )

    # this will cause CMake to not put in the rules that re-run CMake. This might be useful if 
    # you want to use the generated build files on another machine. 
    MESSAGE( STATUS "CMAKE_SUPPRESS_REGENERATION:               " ${CMAKE_SUPPRESS_REGENERATION} )

    # create Makefiles with colored output (defaults to on) 
    MESSAGE( STATUS "CMAKE_COLOR_MAKEFILE:                      " ${CMAKE_COLOR_MAKEFILE} )

    # if set to TRUE, the generated Makefiles will not contain rules for creating preprocessed files (foo.i) 
    MESSAGE( STATUS "CMAKE_SKIP_PREPROCESSED_SOURCE_RULES:      " ${CMAKE_SKIP_PREPROCESSED_SOURCE_RULES} )

    # if set to TRUE, the generated Makefiles will not contain rules for creating compiled, but not yet assembled files (foo.s)
    MESSAGE( STATUS "CMAKE_SKIP_ASSEMBLY_SOURCE_RULES:          " ${CMAKE_SKIP_ASSEMBLY_SOURCE_RULES} )

    MESSAGE( STATUS "CMake compilers" )

    # the compiler used for C files 
    MESSAGE( STATUS "CMAKE_C_COMPILER:                          " ${CMAKE_C_COMPILER} )

    # the compiler used for C++ files 
    MESSAGE( STATUS "CMAKE_CXX_COMPILER:                        " ${CMAKE_CXX_COMPILER} )

    # if the compiler is a variant of gcc, this should be set to 1 
    MESSAGE( STATUS "CMAKE_COMPILER_IS_GNUCC:                   " ${CMAKE_COMPILER_IS_GNUCC} )

    # if the compiler is a variant of g++, this should be set to 1 
    MESSAGE( STATUS "CMAKE_COMPILER_IS_GNUCXX:                  " ${CMAKE_COMPILER_IS_GNUCXX} )

    # if this is set to ON, then all libraries are built as shared libraries by default.
    MESSAGE( STATUS "BUILD_SHARED_LIBS:                         " ${BUILD_SHARED_LIBS} )

    # Choose the type of build.  Example: SET(CMAKE_BUILD_TYPE Debug) 
    MESSAGE( STATUS "CMAKE_BUILD_TYPE:                          " ${CMAKE_BUILD_TYPE} )

    # the tools for creating libraries 
    MESSAGE( STATUS "CMAKE_AR:                                  " ${CMAKE_AR} )
    MESSAGE( STATUS "CMAKE_RANLIB:                              " ${CMAKE_RANLIB} )

    MESSAGE( STATUS "CMAKE_CONFIGURATION_TYPES:                 " "${CMAKE_CONFIGURATION_TYPES}" )
    
    # A simple way to get switches to the compiler is to use ADD_DEFINITIONS(). 
    # But there are also two variables exactly for this purpose: 

    # the compiler flags for compiling C sources 
    MESSAGE( STATUS "CMAKE_C_FLAGS:                             " ${CMAKE_C_FLAGS} )

    MESSAGE( STATUS "CMAKE_C_FLAGS_DEBUG:                       " ${CMAKE_C_FLAGS_DEBUG} )
    MESSAGE( STATUS "CMAKE_C_FLAGS_RELEASE:                     " ${CMAKE_C_FLAGS_RELEASE} )
    MESSAGE( STATUS "CMAKE_C_FLAGS_MINSIZEREL:                  " ${CMAKE_C_FLAGS_MINSIZEREL} )
    MESSAGE( STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO:              " ${CMAKE_C_FLAGS_RELWITHDEBINFO} )

    # the compiler flags for compiling C++ sources 
    MESSAGE( STATUS "CMAKE_CXX_FLAGS:                           " ${CMAKE_CXX_FLAGS} )

    MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG:                     " ${CMAKE_CXX_FLAGS_DEBUG} )
    MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE:                   " ${CMAKE_CXX_FLAGS_RELEASE} )
    MESSAGE( STATUS "CMAKE_CXX_FLAGS_MINSIZEREL:                " ${CMAKE_CXX_FLAGS_MINSIZEREL} )
    MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO:            " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} )

    # what C object files end in. Typically .o or .obj.
    MESSAGE( STATUS "CMAKE_C_OUTPUT_EXTENSION:                  " ${CMAKE_C_OUTPUT_EXTENSION} )
    # meta-variable! Please note that this is an important variable, since on multi-configuration generators
    # it will be generated into dynamically switched content based on the configuration that the user currently
    # selected within the generated build environment. Indicates the name of the current configuration (~ directory)
    # for the project. May be used for any purpose which needs per-configuration-dynamic switching of strings, not
    # just OutputDir configuration. For multi-configuration generators (e.g. MSVC) the resulting strings are typically
    # some of "Debug", "Release", "RelWithDebInfo", or "MinSizeRel". For other compiler generators (single-configuration ones)
    # it is typically ".", as they don't use MSVC-style configuration directories.
    MESSAGE( STATUS "CMAKE_CFG_INTDIR:                          " ${CMAKE_CFG_INTDIR} )
    # additional compiler flags for building shared libraries
    MESSAGE( STATUS "CMAKE_SHARED_LINKER_FLAGS:                  " ${CMAKE_SHARED_LINKER_FLAGS} )

    # # MESSAGE( STATUS ": " ${} )

endfunction()