#include <iostream>

#include "../../generics/generics.h"

using namespace WPEFramework::Core;

void PrintUsage()
{
    std::cout << "pidInfoTest [parent PID]" << std::endl;
}

void ListInfo(int processId)
{
    ProcessInfo processInfo(processId);
    std::cout << "Name: " << processInfo.Name() << std::endl;

    ProcessInfo::Iterator childIterator = processInfo.Children();

    std::cout << "Children (" << childIterator.Count() << "): " << std::endl;
    while (childIterator.Next()) {
        ProcessInfo childProcessInfo = childIterator.Current();
        std::cerr << "name: " << childProcessInfo.Name() << " (" << childProcessInfo.Id() << "): " << childProcessInfo.Resident() << std::endl;
    }
}

int main(int argc, const char* argv[])
{
    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    int processId = atoi(argv[1]);
    ListInfo(processId);

    std::cout << "Done listing" << std::endl;
    return 0;
}
