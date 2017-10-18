set(target_lib_name waylandeglclient)

find_package(Wayland REQUIRED)
find_package(EGL REQUIRED)

if(WAYLAND_FOUND)
    message("-- Wayland support detected")
    include_directories(${WAYLANDSERVER_INCLUDE_DIRS})
    list(APPEND WPEFRAMEWORK_WAYLAND_CLIENT_LIBS ${WAYLAND_LIBRARIES})
    include_directories(${WAYLAND_INCLUDE_DIRS})
endif(WAYLAND_FOUND)

find_package(Westeros)
if(WESTEROS_FOUND)
    message("-- Westeros support detected")
    list(APPEND WPEFRAMEWORK_WAYLAND_CLIENT_SOURCES Westeros.cpp)
    list(APPEND WPEFRAMEWORK_WAYLAND_CLIENT_LIBS ${EGL_LIBRARIES} ${WAYLAND_EGL_LIBRARIES} ${WESTEROS_LIBRARIES})
    include_directories(${WESTEROS_INCLUDE_DIRS})
endif(WESTEROS_FOUND)

if(INSTALL_HEADERS_TO_TARGET)
    install(FILES Client.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/WPEFramework/wayland)
endif(INSTALL_HEADERS_TO_TARGET)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/wayland-egl-client.pc.in"
        "${CMAKE_BINARY_DIR}/wayland-egl-client.pc"
        @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/wayland-egl-client.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")

# add the waylandeglhelper library
add_library(${target_lib_name} SHARED ${WPEFRAMEWORK_WAYLAND_CLIENT_SOURCES})
target_link_libraries(${target_lib_name} ${WPEFRAMEWORK_WAYLAND_CLIENT_LIBS} westeros_simpleshell_client pthread)
set_target_properties(${target_lib_name} PROPERTIES DEFINE_SYMBOL WPEFRAMEWORK_WAYLAND_EGL_CLIENT)
install(TARGETS ${target_lib_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

add_subdirectory (test)
