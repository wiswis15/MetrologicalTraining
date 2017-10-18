#include <iostream>

#include "globals.h"

#include "../../generics/generics.h"
#include "../../rpc/rpc.h"
#include "../../tracing/tracing.h"

int main()
{
   std::cout << "In CommClient test" << std::endl;
   
   WPEFramework::Core::NodeId nodeId(g_hostName.c_str());
   
   WPEFramework::Core::ProxyType<WPEFramework::RPC::CommunicatorClient>
         client(WPEFramework::Core::ProxyType<WPEFramework::RPC::CommunicatorClient>::Create(nodeId));
   
   client->CreateFactory<WPEFramework::RPC::InvokeMessage>(8);

   WPEFramework::Core::ProxyType<WPEFramework::RPC::InvokeServerType<4, 2> > invokeServer =
         WPEFramework::Core::ProxyType<WPEFramework::RPC::InvokeServerType<4, 2> >::Create(0);

   client->Register(WPEFramework::Core::proxy_cast<WPEFramework::Core::IIPCServer>(invokeServer));

   WPEFramework::Core::ProxyType<WPEFramework::RPC::ObjectMessageHandler> objectMessageHandler =
         WPEFramework::Core::ProxyType<WPEFramework::RPC::ObjectMessageHandler>::Create();

   client->Register(WPEFramework::Core::proxy_cast<WPEFramework::Core::IIPCServer>(objectMessageHandler));
   client->CreateFactory<WPEFramework::RPC::ObjectMessage>(2);

   uint32 result = client->Open(WPEFramework::Core::infinite);
   std::cout << "Client.Open(), result: " << result << std::endl;

   std::cout << "Waiting for CommClient to open" << std::endl;
   // TODO: this can be a lot better.
   while (!client->IsOpen());
   std::cout << "CommClient opened" << std::endl;

   TRACE_GLOBAL(WPEFramework::Trace::Information, (_T("%s %d"), _T("Hello World!"), 42));

   std::cout << "Client is sleeping" << std::endl;
   while(true);

   client->Close(WPEFramework::Core::infinite);

   WPEFramework::Core::Singleton::Dispose();
   return 0;
}

