#include <iostream>

#include "../../generics/generics.h"
#include "../../generics/IPCChannel.h"

#include "ThreadedMessageHandlerType.h"
#include "TestMessage.h"
#include "globals.h"

using namespace WPEFramework;

typedef Core::IPCChannelType<Core::SocketPort, Core::Void> MyChannelType;

class TestChannelLink
{
public:
    TestChannelLink(Core::IPCChannelType<Core::SocketPort, TestChannelLink> *p VARIABLE_IS_NOT_USED)
    {
    }
};

class TestChannelServer : public Core::IPCChannelServerType<TestChannelLink, true>
{
    typedef Core::IPCChannelServerType<TestChannelLink, true> BaseClass;

public:
    TestChannelServer(const Core::NodeId& remoteNode)
        : BaseClass(remoteNode, g_communicationBufferSize)
        , _testMessageHandler(Core::ProxyType<RPC::ThreadedMessageHandlerType<RPCTestMessage> >::Create())
    {
        BaseClass::Register(Core::proxy_cast<Core::IIPCServer>(_testMessageHandler));
    }

protected:
    virtual void Added(Core::ProxyType<Client>& client)
    {
        BaseClass::Added(client);

        std::cerr << "TestChannelServer::Added" << std::endl;
    }

    Core::ProxyType<RPC::ThreadedMessageHandlerType<RPCTestMessage> > _testMessageHandler;
};

int main()
{
   std::cout << "In RPC server" << std::endl;

   Core::NodeId node(g_hostName.c_str());

   Core::ProxyType<TestChannelServer> server = Core::ProxyType<TestChannelServer>::Create(node);

   uint32 result = server->Open(Core::infinite);
   std::cerr << "Opened server: " << result << std::endl;

   std::cerr << "Server is going to sleep" << std::endl;
   while(true);

   WPEFramework::Core::Singleton::Dispose();

   return 0;
}
