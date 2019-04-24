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

    Exchange::IAdder * adder = client->Open<Exchange::IAdder>(_T("Adder"));

    cerr << "Client, adder: " << adder << endl;

    cerr << "Client, original value: " << adder->GetValue() << endl;

    cerr << "Client, adding 20..." << endl;
    adder->Add(20);

    cerr << "Client, new value: " << adder->GetValue() << endl;

    cerr << "Client, adding 22..." << endl;
    adder->Add(22);

    cerr << "Client, new value: " << adder->GetValue() << endl;

    cerr << "Client, adder is running in process: " << adder->GetPid() << ", our pid: " << getpid() << endl;

    adder->Release();

    client->Close(Core::infinite);

    WPEFramework::Core::Singleton::Dispose();
}
