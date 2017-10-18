#include <gtest/gtest.h>

#include <iostream>

#include <generics/generics.h>
#include <generics/StreamJSON.h>
#include "DataContainer.h"
//#include "EchoProtocol.h"

namespace WPEFramework {
namespace Core {
namespace Test {

class JSONObjectFactory : public Core::FactoryType<Core::JSON::IElement, string> {
private:
    inline JSONObjectFactory()
    : Core::FactoryType<Core::JSON::IElement, string>()
    {
    }
    JSONObjectFactory(const JSONObjectFactory&);
    JSONObjectFactory& operator=(const JSONObjectFactory&);

public:
    static JSONObjectFactory& Instance()
    {
        static JSONObjectFactory _singleton;

        return (_singleton);
    }
    virtual ~JSONObjectFactory()
    {
    }
};

class TextConnector : public Core::StreamTextType<Core::SocketStream, Core::TerminatorCarriageReturn>
{
private:
    typedef Core::StreamTextType <Core::SocketStream, Core::TerminatorCarriageReturn> BaseClass;

    TextConnector();

    TextConnector(const TextConnector &copy);

    TextConnector &operator=(const TextConnector &);

public:
    TextConnector(const WPEFramework::Core::NodeId &remoteNode)
    : BaseClass(false, remoteNode.AnyInterface(), remoteNode, 1024, 1024), _serverSocket(false),
      _dataPending(false, false)
    {
    }

    TextConnector(const SOCKET &connector, const Core::NodeId &remoteId,
                  Core::SocketServerType <TextConnector> &)
    : BaseClass(false, connector, remoteId, 1024, 1024), _serverSocket(true), _dataPending(false, false)
    {
    }

    virtual ~TextConnector()
    {
        Close(WPEFramework::Core::infinite);
    }

public:
    virtual void Received(string &text)
    {
        printf(_T("Received: %s\n"), text.c_str());
        printf(_T("   Bytes: %d\n"), static_cast<uint32>(text.size()));

        // prevent singing around, only sockets created in a serve context should reply!!
        if (_serverSocket == true)
        {
#ifdef __WIN32__
#pragma warning(disable : 4996)
#endif

            TCHAR buffer[128];
            sprintf(buffer, _T("Handled [%d] bytes."), static_cast<uint32>(text.size()));

#ifdef __WIN32__
#pragma warning(default : 4996)
#endif

            //Submit(string(buffer));
            Submit(text);
        } else
        {
            _dataReceived = text;
            _dataPending.SetEvent();
        }
    }

    int Wait(unsigned int milliseconds) const
    {
        return _dataPending.Lock(milliseconds);
    }

    bool Retrieve(string &text)
    {
        text = _dataReceived;
        bool isEmpty = _dataReceived.empty();
        // clear internal receive buffer once obtained
        _dataReceived.clear();
        return !isEmpty;
    }

    virtual void Send(const string &text)
    {
        printf(_T("Bytes: %d\n"), static_cast<uint32>(text.size()));
        printf(_T(" Send: %s\n"), text.c_str());
    }

    virtual void StateChange()
    {
        printf(_T("State change: "));
        if (IsOpen())
        {
            printf(_T("[1] Open - OK\n"));
        } else
        {
            printf(_T("[1] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
        }
    }

private:
    bool _serverSocket;
    string _dataReceived;
    mutable WPEFramework::Core::Event _dataPending;
};

class JSONConnector : public Core::StreamJSONType<Core::SocketStream, JSONObjectFactory &>
{
private:
    typedef Core::StreamJSONType<Core::SocketStream, JSONObjectFactory &> BaseClass;

    JSONConnector();

    JSONConnector(const JSONConnector &copy);

    JSONConnector &operator=(const JSONConnector &);

public:
    JSONConnector(const WPEFramework::Core::NodeId &remoteNode)
    : BaseClass(5, JSONObjectFactory::Instance(), false, remoteNode.AnyInterface(), remoteNode, 1024, 1024),
      _serverSocket(false), _dataPending(false, false)
    {
    }

    JSONConnector(const SOCKET &connector, const Core::NodeId &remoteId,
                  Core::SocketServerType <JSONConnector> &)
    : BaseClass(5, JSONObjectFactory::Instance(), false, connector, remoteId, 1024, 1024), _serverSocket(true),
      _dataPending(false, false)
    {
    }

    virtual ~JSONConnector()
    {
        Close(WPEFramework::Core::infinite);
    }

public:
    virtual void Received(Core::ProxyType <Core::JSON::IElement> &newElement)
    {
        string textElement;
        newElement->ToString(textElement);

        printf(_T("   Bytes: %d\n"), static_cast<uint32>(textElement.size()));
        printf(_T("Received: %s\n"), textElement.c_str());

        // prevent singing around, only sockets created in a serve context should reply!!
        if (_serverSocket == true)
        {
            // As this is the server, send back the Element we received...
            Submit(newElement);
        } else
        {
            _dataReceived = textElement;
            _dataPending.SetEvent();
        }
    }

    int Wait(unsigned int milliseconds) const
    {
        return _dataPending.Lock(milliseconds);
    }

    bool Retrieve(Core::ProxyType <Core::JSON::IElement> &&newElement)
    {
        newElement->FromString(_dataReceived);
        bool isEmpty = _dataReceived.empty();
        // clear internal receive buffer once obtained
        _dataReceived.clear();
        return !isEmpty;
    }

    virtual void Send(Core::ProxyType <Core::JSON::IElement> &newElement)
    {
        string textElement;
        newElement->ToString(textElement);

        printf(_T("Bytes: %d\n"), static_cast<uint32>(textElement.size()));
        printf(_T(" Send: %s\n"), textElement.c_str());
    }

    virtual void StateChange()
    {
        printf(_T("State change: "));
        if (IsOpen())
        {
            printf(_T("[2] Open - OK\n"));
        } else
        {
            printf(_T("[2] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
        }
    }

    virtual bool IsIdle() const
    {
        return (true);
    }

private:
    bool _serverSocket;
    string _dataReceived;
    mutable WPEFramework::Core::Event _dataPending;
};

class StreamTest : public ::testing::Test {
public:
    StreamTest()
        : textConnector(NodeId("127.0.0.1", 12341))
        , jsonConnector(NodeId("127.0.0.1", 12342))
    {
    }

protected:
    virtual void SetUp() {
        textConnector.Open(0);
        jsonConnector.Open(0);
        //  webConnector.Open(0);
        //  jsonWebConnector.Open(0);
        //  webSocketLocalConnection.Open(0);
        //  webSocketJSONConnection.Open(0);
        //  webSocketRemoteConnection.Open(0);
    }

    virtual void TearDown() {
        std::cout << "Just before" << std::endl;
        textConnector.Close(0);
        std::cout << "Just after" << std::endl;
        jsonConnector.Close(0);
        std::cout << "Ending it all" << std::endl;
        //  webConnector.Close(0);
        //  jsonWebConnector.Close(0);
        //  webSocketLocalConnection.Close(0);
        //  webSocketJSONConnection.Close();
        //  webSocketRemoteConnection.Close(0);
    }

    TextConnector textConnector;
    JSONConnector jsonConnector;
};

// These test are temporarily disabled, as they require a server to be running, which still has to be integrated.
//TEST_F(StreamTest, TextConnector)
//{
//    std::string message;
//
//    for (int teller = 0; teller < 1600; teller++) {
//        message += static_cast<TCHAR>('0' + (teller % 10));
//    }
//
//    textConnector.Submit(message);
//
//    // Give server some time to respond so wait 500ms
//    textConnector.Wait(500);
//
//    std::string response;
//    bool hasData = textConnector.Retrieve(response);
//
//    EXPECT_TRUE(hasData);
//    EXPECT_EQ(message, response);
//}
//
//TEST_F(StreamTest, JSONConnector)
//{
//    // Factories for JSON objects..
//    JSONObjectFactory::Instance().CreateFactory<JSON::LabelType<TestSupport::DataContainer::Command> >(_T(""), 5);
//
//    ProxyType<JSON::LabelType<TestSupport::DataContainer::Command> > sendObject = ProxyType<JSON::LabelType<TestSupport::DataContainer::Command> >::Create();
//
//    sendObject->Identifier = 11;
//    sendObject->Name = _T("TestCommand");
//    jsonConnector.Submit(proxy_cast<JSON::IElement>(sendObject));
//
//    jsonConnector.Wait(1000);
//
//    ProxyType<JSON::LabelType<TestSupport::DataContainer::Command> > responseObject = ProxyType<JSON::LabelType<TestSupport::DataContainer::Command> >::Create();
//    bool hasData = jsonConnector.Retrieve(proxy_cast<JSON::IElement>(responseObject));
//
//    // NOTE: And here something magics happens by disposing any created Singleton
//    //    Effectively preventing a core dump/segmentation fault
//
//    EXPECT_TRUE(hasData);
//    EXPECT_EQ(sendObject->Identifier, responseObject->Identifier);
//    //  EXPECT_EQ(sendObject->Name, responseObject->Name);
//
//    JSONObjectFactory::Instance().DestroyFactory(JSON::LabelType<TestSupport::DataContainer::Command>::Id());
//}

} // namespace Test
} // namespace Core
} // namespace WPEFramework
