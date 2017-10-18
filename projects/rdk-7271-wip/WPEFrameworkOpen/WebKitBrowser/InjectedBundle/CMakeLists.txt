find_package(PkgConfig)
pkg_check_modules(PC_WPEFRAMEWORK WPEFramework)

find_library(
   WPEFRAMEWORKPROTOCOLS_LIBRARY
   NAMES WPEFrameworkProtocols
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

find_library(
   WPEFRAMEWORKCORE_LIBRARY
   NAMES WPEFrameworkCore
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

find_library(
   WPEFRAMEWORKTRACING_LIBRARY
   NAMES WPEFrameworkTracing
   HINTS ${PC_WPEFRAMEWORK_LIBDIR} ${PC_WPEFRAMEWORK_LIBRARY_DIRS}
)

add_library(WPEInjectedBundle SHARED
    main.cpp
    WhiteListedOriginDomainsList.cpp
    Utils.cpp
    JavaScriptFunction.cpp
    NotifyWPEFramework.cpp
    Milestone.cpp
    ClassDefinition.cpp
)

target_include_directories(WPEInjectedBundle PUBLIC
    ${WPE_WEBKIT_INCLUDE_DIRS}
)
target_link_libraries(WPEInjectedBundle
    ${WPE_WEBKIT_LIBRARIES} 
    ${WPEFRAMEWORKPROTOCOLS_LIBRARY}
    ${WPEFRAMEWORKTRACING_LIBRARY}
    ${WPEFRAMEWORKCORE_LIBRARY}
)

install(TARGETS WPEInjectedBundle DESTINATION "${CMAKE_INSTALL_PREFIX}/share/WPEFramework/WebKitBrowser")
