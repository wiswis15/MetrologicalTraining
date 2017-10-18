#include <iostream>

#include "generics.h"

int main(int argc VARIABLE_IS_NOT_USED, char **argv VARIABLE_IS_NOT_USED)
{
    std::cout << "getDeviceId Test:" << std::endl;

    WPEFramework::Core::Time newTime (WPEFramework::Core::Time::Now());
    newTime.Add (1000);

    struct tm setTime;

    ::memcpy(&setTime, &(newTime.Handle()), sizeof (setTime));

    char buffer[64];
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", &(newTime.Handle()));
    printf ("Time: %s\n", buffer); 

    time_t value = mktime(&setTime);

    int result;

    if ((result = stime(&value)) == -1) {
        TRACE_L1("Failed to set system time [%d] %d ", errno, result);
    }
    else {
        TRACE_L1("System time updated [%d]", errno);
    }

    WPEFramework::Core::SystemInfo::Instance().SetTime (newTime);

    WPEFramework::Core::Singleton::Dispose();

    std::cout << "Done..." << std::endl;
    return (0);
}
