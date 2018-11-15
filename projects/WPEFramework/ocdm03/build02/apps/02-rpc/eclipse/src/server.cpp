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
public:
    Adder()
        : m_value(0)
    {
    }

    uint32_t GetValue()
    {
        return m_value;
    }

    void Add(uint32_t value)
    {
        m_value += value;
    }

    pid_t GetPid()
    {
        return getpid();
    }

    BEGIN_INTERFACE_MAP(Adder)
        INTERFACE_ENTRY(Exchange::IAdder)
    END_INTERFACE_MAP

private:
    uint32_t m_value;
};

// TODO: can we straight-away use PRC::Administrator?
class ExternalAccess : public RPC::Communicator
{
private:
    ExternalAccess() = delete;
    ExternalAccess(const ExternalAccess &) = delete;
    ExternalAccess & operator=(const ExternalAccess &) = delete;

public:
    ExternalAccess(const Core::NodeId & source)
        : RPC::Communicator(source, Core::ProxyType< RPC::InvokeServerType<4, 1> >::Create(), _T(""))
    {
        Open(Core::infinite);
    }

    ~ExternalAccess()
    {
        Close(Core::infinite);
    }

private:
    virtual void* Aquire(const string& className, const uint32_t interfaceId, const uint32_t versionId)
    {
        void* result = nullptr;

        if (interfaceId == Exchange::IAdder::ID) {
            Exchange::IAdder * newAdder = Core::Service<Adder>::Create<Exchange::IAdder>();
            result = newAdder;
        }

        return result;
    }
};

int main()
{
    cerr << "Server PID: " << getpid() << endl;

    Core::NodeId remoteNode(g_connectorName.c_str());

    ExternalAccess communicator(remoteNode);

    cerr << "Server, communicator is listening: " << communicator.IsListening() << endl;

    sleep(10);

    WPEFramework::Core::Singleton::Dispose();
}
