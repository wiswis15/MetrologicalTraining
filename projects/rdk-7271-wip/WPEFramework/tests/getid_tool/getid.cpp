#include <iostream>

#include "generics.h"

int main(int argc VARIABLE_IS_NOT_USED, char **argv VARIABLE_IS_NOT_USED)
{

    const uint8* rawDeviceId (WPEFramework::Core::SystemInfo::Instance().RawDeviceId());

    string id (WPEFramework::Core::SystemInfo::Instance().Id(rawDeviceId, 0xFF));

    std::cout << "This is your device ID:" << std::endl
              << id << std::endl
              << "You can request provisioning using this ID..." << std::endl;

    WPEFramework::Core::Singleton::Dispose();
    return (0);
}
