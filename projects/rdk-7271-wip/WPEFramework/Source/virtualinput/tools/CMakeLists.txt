#
# virtual input monitor
#
set(VK_MONITOR_SOURCES_CTEST
        vk_monitor.c
        )

set(VK_MONITOR_LIBRARIES_CTEST
        virtualinput
        )

add_executable(vk_monitor ${VK_MONITOR_SOURCES_CTEST})
target_link_libraries(vk_monitor ${VK_MONITOR_LIBRARIES_CTEST})

install(TARGETS vk_monitor DESTINATION /usr/bin/)


