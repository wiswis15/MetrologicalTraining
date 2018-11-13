#include <WPEFramework/core/core.h>
#include <WPEFramework/com/com.h>

#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#include "globals.h"
#include "interfaces.h"

using namespace WPEFramework;
using namespace std;

class Adder : public Exchange::IAdder
{

    BEGIN_INTERFACE_MAP(Adder)
        INTERFACE_ENTRY(Exchange::IAdder)
    END_INTERFACE_MAP

};

int main()
{
    cerr << "Server PID: " << getpid() << endl;

    Core::NodeId remoteNode(g_connectorName.c_str());
    RPC::Communicator communicator(remoteNode, Core::ProxyType< RPC::InvokeServerType<4, 1> >::Create(), _T(""));

    Exchange::IAdder * localAdder = Core::Service<Adder>::Create<Exchange::IAdder>();

    //cerr << "Server is in infinite loop" << endl;

    //while(true);

    sleep(10);

    WPEFramework::Core::Singleton::Dispose();
}
