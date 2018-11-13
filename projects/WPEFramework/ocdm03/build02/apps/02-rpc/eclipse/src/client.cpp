#include <WPEFramework/com/com.h>

#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#include "globals.h"

using namespace WPEFramework;
using namespace std;

int main()
{
    sleep(1);

    cerr << "Client PID: " << getpid() << endl;


    Core::NodeId remoteNode(g_connectorName.c_str());
    Core::ProxyType<RPC::IHandler> handler(Core::ProxyType<RPC::InvokeServerType<4,1> >::Create(Core::Thread::DefaultStackSize()));

    Core::ProxyType<RPC::CommunicatorClient> client(Core::ProxyType<RPC::CommunicatorClient>::Create(remoteNode, handler));

    client->Open();




    WPEFramework::Core::Singleton::Dispose();
}
