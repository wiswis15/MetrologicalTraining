#include <iostream>
#include <string>

#include "generics.h"

int main(int argc, char** argv)
{
    std::cout << "getDeviceId Test:" << std::endl;

    for (int a = 0; a < 20; a++) {
        std::cout << a << ". Test getDeviceid: " << WPEFramework::Core::DeviceId::Instance().GetDeviceid() << std::endl;
        ;
    }

    std::cout << "Test getSystemPrefix: " << WPEFramework::Core::DeviceId::Instance().GetSystemPrefix() << std::endl;

    WPEFramework::Core::Singleton::Dispose();

    std::cout << "Done..." << std::endl;
    return (0);
}
