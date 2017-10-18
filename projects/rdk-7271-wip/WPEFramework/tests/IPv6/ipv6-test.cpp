#include <iostream>

#include "generics.h"

using namespace WPEFramework;

static void printNodeIdInfo(const char hostName[])
{
   Core::NodeId nodeId(hostName);

   std::cout << "*** " << hostName << " ***" << std::endl;
   std::cout << "Hostname: " << nodeId.HostName() << std::endl;
   std::cout << "Type: " << nodeId.Type() << std::endl;
   std::cout << "PortNumber: " << nodeId.PortNumber() << std::endl;
   std::cout << "IsValid: " << nodeId.IsValid() << std::endl;
   std::cout << "Size: " << nodeId.Size() << std::endl;
   std::cout << "HostAddress: " << nodeId.HostAddress() << std::endl;
   std::cout << "QualifiedName: " << nodeId.QualifiedName() << std::endl;
   std::cout << "IsLocalInterface: " << nodeId.IsLocalInterface() << std::endl;
   std::cout << "IsAnyInterface: " << nodeId.IsAnyInterface() << std::endl;
   std::cout << "IsMulticast: " << nodeId.IsMulticast() << std::endl;
}

int main()
{
   std::cout << std::boolalpha;

   printNodeIdInfo("127.0.0.1:80");
   printNodeIdInfo("localhost:80");

   return 0;
}

