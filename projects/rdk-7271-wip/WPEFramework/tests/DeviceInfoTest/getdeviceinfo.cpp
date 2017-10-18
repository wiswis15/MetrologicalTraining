#include <iostream>

#include "../generics/generics.h"
#include "../tracing/tracing.h"

int main(int argc VARIABLE_IS_NOT_USED, char **argv VARIABLE_IS_NOT_USED)
{
    const uint8* rawDeviceId (WPEFramework::Core::SystemInfo::Instance().RawDeviceId());

    string id (WPEFramework::Core::SystemInfo::Instance().Id(rawDeviceId, 0xFF));

    std::cout << "SystemInfo Test:" << std::endl;

    std::cout << "Test GetId: " << id << std::endl;

    // std::cout << "Test GetPrefix: " << WPEFramework::Core::SystemInfo::Instance().GetPrefix() << std::endl;

    std::cout << "Test GetHostname: " << WPEFramework::Core::SystemInfo::Instance().GetHostName() << std::endl;

    std::cout << "Test GetUpTime: " << WPEFramework::Core::SystemInfo::Instance().GetUpTime() << std::endl;

    std::cout << "Test GetTotalRam: " << WPEFramework::Core::SystemInfo::Instance().GetTotalRam() << std::endl;

    std::cout << "Test GetFreeRam: " << WPEFramework::Core::SystemInfo::Instance().GetFreeRam() << std::endl;

    for (int a = 0; a < 3; a++) {
        std::cout << a << ". Test GetCpuLoad: " << WPEFramework::Core::SystemInfo::Instance().GetCpuLoad() << std::endl;
    }


    TRACE_GLOBAL(WPEFramework::Trace::Information, (_T("poep snikkel")));
    
    char value;

    std::cin >> value;

    std::cout << "You typed: " << value << std::endl;

    WPEFramework::Core::Singleton::Dispose();

    std::cout << "Done..." << std::endl;
    return (0);
}
