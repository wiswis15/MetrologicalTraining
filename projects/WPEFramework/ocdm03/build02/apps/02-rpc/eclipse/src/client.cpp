#include <WPEFramework/com/com.h>

#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#include "globals.h"
#include "interfaces.h"

using namespace WPEFramework;
using namespace std;

int main()
{
    sleep(1);

    cerr << "Client PID: " << getpid() << endl;

    Core::NodeId remoteNode(g_connectorName.c_str());
    Core::ProxyType<RPC::IHandler> handler(Core::ProxyType<RPC::InvokeServerType<4,1> >::Create(Core::Thread::DefaultStackSize()));

    Core::ProxyType<RPC::CommunicatorClient> client(Core::ProxyType<RPC::CommunicatorClient>::Create(remoteNode, handler));

    sleep(1);

    cerr << "Client, is valid: " << client.IsValid() << endl;

    sleep(1);

    cerr << "Client, about to call Open" << endl;

    Exchange::IAdder * adder = client->Open<Exchange::IAdder>(_T("Adder"));

    cerr << "Client, adder: " << adder << endl;





    WPEFramework::Core::Singleton::Dispose();
}
