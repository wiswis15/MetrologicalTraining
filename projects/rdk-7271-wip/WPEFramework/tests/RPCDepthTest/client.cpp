#include <iostream>

#include "../../generics/generics.h"
#include "../../generics/IPCChannel.h"

#include "ThreadedMessageHandlerType.h"
#include "TestMessage.h"
#include "globals.h"

using namespace WPEFramework;

volatile bool g_open = false;

// TODO: "internal factory"?
class TestClient : public Core::IPCChannelClientType<Core::Void, false, true>, public Core::IDispatchType<Core::IIPC>
{
private:
    typedef Core::IPCChannelClientType<Core::Void, false, true> BaseClass;

public:
    TestClient(const Core::NodeId& node)
       : Core::IPCChannelClientType<Core::Void, false, true>(node, g_communicationBufferSize)
    {

    }

    virtual void Dispatch(Core::IIPC &element VARIABLE_IS_NOT_USED)
    {
        std::cerr << "TestClient::Dispatch" << std::endl;
    }

    virtual void StateChange()
    {
        BaseClass::StateChange();

        std::cerr << "TestClient::StateChange: " << IsOpen() << std::endl;

        if (IsOpen()) {
            g_open = true;
        }
    }

    Core::ProxyType<RPC::ThreadedMessageHandlerType<RPCTestMessage> > _testMessageHandler;
};

int main()
{
   Core::NodeId node(g_hostName.c_str());

   Core::ProxyType<TestClient> client = Core::ProxyType<TestClient>::Create(node);

   uint32_t result = client->Open(Core::infinite);
   std::cerr << "Client->Open: " << result << std::endl;

   std::cerr << "Client is waiting for state change" << std::endl;
   while(!g_open);

   Core::ProxyType<RPCTestMessage> message = Core::ProxyType<RPCTestMessage>::Create();

   std::cerr << "Before invoke" << std::endl;
   client->Invoke(message, Core::infinite);
   std::cerr << "After invoke" << std::endl;

   std::cerr << "Client is going to sleep" << std::endl;
   while(true);

   WPEFramework::Core::Singleton::Dispose();

}
