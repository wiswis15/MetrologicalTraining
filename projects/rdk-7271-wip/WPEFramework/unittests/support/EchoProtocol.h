#ifndef __ECHOPROTOCOL_H
#define __ECHOPROTOCOL_H

#include <generics/generics.h>
#include <generics/Sync.h>
//#include <websocket/websocket.h>

#include "DataContainer.h"

namespace WPEFramework {
namespace TestSupport {
namespace EchoProtocol {
//    class StressTextFactory {
//    private:
//        StressTextFactory(const StressTextFactory&);
//        StressTextFactory& operator=(const StressTextFactory&);
//
//    public:
//        StressTextFactory()
//            : _minLength(static_cast<uint16>(~0))
//            , _maxLength(0)
//        {
//            _testSequence = "theTHEquickQUICKbrownBROWNfoxFOXjumpsJUMPSoverOVERlazyLAZYdogDOG!";
//        }
//        ~StressTextFactory()
//        {
//        }
//
//    public:
//        inline uint16 MinSize() const
//        {
//            return (_minLength);
//        }
//        inline uint16 MaxSize() const
//        {
//            return (_maxLength);
//        }
//        string GenerateData()
//        {
//            // Generate the message
//            uint16 index = 0;
//            uint16 requiredLength;
//            Crypto::Random(requiredLength, static_cast<const uint16>(8), static_cast<const uint16>(2048));
//            string myMessage = Core::NumberType<uint16, false, BASE_HEXADECIMAL>(requiredLength).Text();
//
//            if (requiredLength < _minLength) {
//                _minLength = requiredLength;
//            }
//            if (requiredLength > _maxLength) {
//                _maxLength = requiredLength;
//            }
//            myMessage += " ";
//
//            while (myMessage.length() < requiredLength) {
//                myMessage += _testSequence[index & 0x3F];
//                index++;
//            }
//
//            return (myMessage);
//        }
//        bool VerifyData(const string& data) const
//        {
//            bool result = false;
//
//            // Length should be in the beginning, validate that firs
//            Core::NumberType<uint16, false> expectedLength(data.c_str(), 7, BASE_HEXADECIMAL);
//
//            if (expectedLength.Value() == data.length()) {
//                const TCHAR* buffer = data.c_str();
//                uint16 lengthToValidate = 0;
//
//                // Oke length is oke, now check the pattern..
//                while ((lengthToValidate <= 7) && (*buffer != ' ')) {
//                    lengthToValidate++;
//                    buffer++;
//                }
//
//                if (*buffer == ' ') {
//                    // We are still good to go !!!
//                    lengthToValidate = data.length() - lengthToValidate - 1;
//                    buffer++;
//                    result = true;
//
//                    while ((lengthToValidate-- != 0) && (result == true)) {
//                        result = (buffer[lengthToValidate] == _testSequence[lengthToValidate & 0x3F]);
//                    }
//
//                    if (result == false) {
//                        _minLength += 1;
//                    }
//                }
//            }
//
//            if (result == false) {
//                _minLength -= 1;
//            }
//
//            return (result);
//        }
//
//    private:
//        const TCHAR* _testSequence;
//        mutable uint16 _minLength;
//        uint16 _maxLength;
//    };
//
// -----------------------------------------------------------------------------------------------
// Create a resource allocator for all JSON objects used in these tests
// -----------------------------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------------------------
// TEST SET 1
// ----------
// Direct connection for transfering text in an echo fashion.
// -----------------------------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------------------------
// TEST SET 2
// ----------
// Direct connection for transfering JSON in an echo fashion.
// -----------------------------------------------------------------------------------------------
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

//    // -----------------------------------------------------------------------------------------------
//    // TEST SET 3
//    // ----------
//    // Basic Web Echo Service, What is coming in, is send out (WebServer)
//    // Basic Web Client connector. Allows you to send out a Request and receive response (WebClient)
//    // -----------------------------------------------------------------------------------------------
//    class EXTERNAL WebServer : public Web::WebLinkType<Core::SocketStream, Web::Request, Web::Response, WPEFramework::Core::ProxyPoolType<Web::Request> > {
//    private:
//        typedef Web::WebLinkType<Core::SocketStream, Web::Request, Web::Response, WPEFramework::Core::ProxyPoolType<Web::Request> > BaseClass;
//
//        WebServer();
//        WebServer(const WebServer& copy);
//        WebServer& operator=(const WebServer&);
//
//    public:
//        WebServer(const SOCKET& connector, const Core::NodeId& remoteId, Core::SocketServerType<WebServer>&)
//            : BaseClass(5, false, connector, remoteId, 2048, 2048)
//        {
//        }
//        virtual ~WebServer()
//        {
//            Close(WPEFramework::Core::infinite);
//        }
//
//    public:
//        // Notification of a Partial Request received, time to attach a body..
//        virtual void LinkBody(Core::ProxyType<WPEFramework::Web::Request>& element)
//        {
//            // Time to attach a String Body
//            element->Body<Web::TextBody>(_textBodyFactory.Element());
//        }
//        virtual void Received(Core::ProxyType<WPEFramework::Web::Request>& request)
//        {
//            string received;
//            request->ToString(received);
//            printf(_T("Received: %s\n"), received.c_str());
//
//            // What we get in, we should send it back :-)
//            Core::ProxyType<WPEFramework::Web::Response> response = _responseFactory.Element();
//            Core::ProxyType<Web::TextBody> textBody = request->Body<Web::TextBody>();
//
//            // We attached it so it should be available!!!
//            ASSERT(textBody.IsValid());
//
//            response->Date = Core::Time::Now();
//            response->ErrorCode = 200;
//            *textBody = "Here is your stuff back [" + *textBody + "]";
//            response->Body<Web::TextBody>(textBody);
//            Submit(response);
//        }
//        virtual void Send(const Core::ProxyType<WPEFramework::Web::Response>& response)
//        {
//            string send;
//            response->ToString(send);
//            printf(_T("Send: %s\n"), send.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[3] Open - OK\n"));
//            }
//            else {
//                printf(_T("[3] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//
//    private:
//        static Core::ProxyPoolType<WPEFramework::Web::Response> _responseFactory;
//        static Core::ProxyPoolType<Web::TextBody> _textBodyFactory;
//    };
//
//    class EXTERNAL WebClient : public Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request, WPEFramework::Core::ProxyPoolType<Web::Response>&> {
//    private:
//        typedef Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request, WPEFramework::Core::ProxyPoolType<Web::Response>&> BaseClass;
//
//        // All requests needed by any instance of this socket class, is coming from this static factory.
//        // This means that all Requests, received are shared among all WebServer sockets, hopefully limiting
//        // the number of requests that need to be created.
//        static WPEFramework::Core::ProxyPoolType<Web::Response> _responseFactory;
//
//        WebClient();
//        WebClient(const WebClient& copy);
//        WebClient& operator=(const WebClient&);
//
//    public:
//        WebClient(const WPEFramework::Core::NodeId& remoteNode)
//            : BaseClass(5, _responseFactory, false, Core::NodeId::AnyInterface(), remoteNode, 2048, 208)
//        {
//        }
//        virtual ~WebClient()
//        {
//            Close(WPEFramework::Core::infinite);
//        }
//
//    public:
//        // Notification of a Partial Request received, time to attach a body..
//        virtual void LinkBody(Core::ProxyType<WPEFramework::Web::Response>& element)
//        {
//            // Time to attach a String Body
//            element->Body<Web::TextBody>(_textBodyFactory.Element());
//        }
//        virtual void Received(Core::ProxyType<WPEFramework::Web::Response>& element)
//        {
//            string received;
//            element->ToString(received);
//            printf(_T("Received: %s\n"), received.c_str());
//        }
//        virtual void Send(const Core::ProxyType<WPEFramework::Web::Request>& response)
//        {
//            string send;
//            response->ToString(send);
//            printf(_T("Send: %s\n"), send.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[3] Open - OK\n"));
//            }
//            else {
//                printf(_T("[3] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//        static Core::ProxyPoolType<Web::TextBody> _textBodyFactory;
//    };
//
//    // -----------------------------------------------------------------------------------------------
//    // TEST SET 4
//    // ----------
//    // JSON Webrequest server, composit JSON object to the request/response. (JSONWebServer)
//    // JSON Webrequest client, composit JSON object to the request/response. (JSONWebClient)
//    // -----------------------------------------------------------------------------------------------
//    typedef WPEFramework::Web::JSONBodyType<WPEFramework::DataContainer::Command> CommandBody;
//
//    class JSONWebServer : public Web::WebLinkType<Core::SocketStream, Web::Request, Web::Response, WPEFramework::Core::ProxyPoolType<Web::Request>&> {
//    private:
//        typedef Web::WebLinkType<Core::SocketStream, Web::Request, Web::Response, WPEFramework::Core::ProxyPoolType<Web::Request>&> BaseClass;
//
//        // All requests needed by any instance of this socket class, is coming from this static factory.
//        // This means that all Requests, received are shared among all WebServer sockets, hopefully limiting
//        // the number of requests that need to be created.
//        static WPEFramework::Core::ProxyPoolType<Web::Request> _requestFactory;
//        static WPEFramework::Core::ProxyPoolType<CommandBody> _commandBodyFactory;
//
//        JSONWebServer();
//        JSONWebServer(const JSONWebServer& copy);
//        JSONWebServer& operator=(const JSONWebServer&);
//
//    public:
//        JSONWebServer(const SOCKET& connector, const Core::NodeId& remoteId, Core::SocketServerType<JSONWebServer>&)
//            : BaseClass(5, _requestFactory, false, connector, remoteId, 2048, 2048)
//        {
//        }
//        virtual ~JSONWebServer()
//        {
//            Close(WPEFramework::Core::infinite);
//        }
//
//    public:
//        // Notification of a Partial Request received, time to attach a body..
//        virtual void LinkBody(Core::ProxyType<WPEFramework::Web::Request>& element)
//        {
//            // Time to attach a Command Body
//            element->Body<CommandBody>(_commandBodyFactory.Element());
//        }
//        virtual void Received(Core::ProxyType<Web::Request>& request)
//        {
//            string received;
//            request->ToString(received);
//            printf(_T("Received: %s\n"), received.c_str());
//        }
//        virtual void Send(const Core::ProxyType<Web::Response>& response)
//        {
//            string send;
//            response->ToString(send);
//            printf(_T("Send: %s\n"), send.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[4] Open - OK\n"));
//            }
//            else {
//                printf(_T("[4] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//    };
//
//    class JSONWebClient : public Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request, WPEFramework::Core::ProxyPoolType<Web::Response>&> {
//    private:
//        typedef Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request, WPEFramework::Core::ProxyPoolType<Web::Response>&> BaseClass;
//
//        // All requests needed by any instance of this socket class, is coming from this static factory.
//        // This means that all Requests, received are shared among all WebServer sockets, hopefully limiting
//        // the number of requests that need to be created.
//        static WPEFramework::Core::ProxyPoolType<Web::Response> _responseFactory;
//        static WPEFramework::Core::ProxyPoolType<CommandBody> _commandBodyFactory;
//
//        JSONWebClient();
//        JSONWebClient(const JSONWebClient& copy);
//        JSONWebClient& operator=(const JSONWebClient&);
//
//    public:
//        JSONWebClient(const WPEFramework::Core::NodeId& remoteNode)
//            : BaseClass(5, _responseFactory, false, Core::NodeId::AnyInterface(), remoteNode, 2048, 208)
//        {
//        }
//        virtual ~JSONWebClient()
//        {
//            Close(WPEFramework::Core::infinite);
//        }
//
//    public:
//        // Notification of a Partial Request received, time to attach a body..
//        virtual void LinkBody(Core::ProxyType<WPEFramework::Web::Response>& element)
//        {
//            // Time to attach a Command Body
//            element->Body<CommandBody>(_commandBodyFactory.Element());
//        }
//        virtual void Received(Core::ProxyType<Web::Response>& response)
//        {
//            string received;
//            response->ToString(received);
//            printf(_T("Received: %s\n"), received.c_str());
//        }
//        virtual void Send(const Core::ProxyType<Web::Request>& request)
//        {
//            string send;
//            request->ToString(send);
//            printf(_T("Send: %s\n"), send.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[4] Open - OK\n"));
//            }
//            else {
//                printf(_T("[4] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//    };
//
//    // -----------------------------------------------------------------------------------------------
//    // TEST SET 5
//    // ----------
//    // WebSocket Echo service (TEXT), send back what we got + postfix. This is the native WebSockets.
//    // -----------------------------------------------------------------------------------------------
//    class EchoWebSocketServer : public Core::StreamTextType<Web::WebSocketServerType<Core::SocketStream>, Core::TerminatorCarriageReturn> {
//    private:
//        typedef Core::StreamTextType<Web::WebSocketServerType<Core::SocketStream>, Core::TerminatorCarriageReturn> BaseClass;
//
//    private:
//        EchoWebSocketServer(const EchoWebSocketServer&);
//        EchoWebSocketServer& operator=(const EchoWebSocketServer&);
//
//    public:
//        EchoWebSocketServer(const SOCKET& socket, const WPEFramework::Core::NodeId& remoteNode, WPEFramework::Core::SocketServerType<EchoWebSocketServer>&)
//            : BaseClass(false, true, false, socket, remoteNode, 1024, 1024)
//        {
//        }
//        virtual ~EchoWebSocketServer()
//        {
//        }
//
//    private:
//        virtual void Received(string& text)
//        {
//            printf(_T("Received: %s\n"), text.c_str());
//
//            // Send back what we receive
//            Submit(_T("Here is your shit back [") + text + _T("]"));
//        }
//        virtual void Send(const string& text)
//        {
//            printf(_T("Send: %s\n"), text.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[5] Open - OK\n"));
//            }
//            else {
//                printf(_T("[5] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//    };
//
//    class EchoWebSocketClient : public Core::StreamTextType<Web::WebSocketClientType<Core::SocketStream>, Core::TerminatorCarriageReturn> {
//    private:
//        typedef Core::StreamTextType<Web::WebSocketClientType<Core::SocketStream>, Core::TerminatorCarriageReturn> BaseClass;
//
//    private:
//        EchoWebSocketClient(const EchoWebSocketClient&);
//        EchoWebSocketClient& operator=(const EchoWebSocketClient&);
//
//    public:
//        EchoWebSocketClient(const Core::NodeId& remoteNode)
//            : BaseClass(_T("/"), _T("echo"), _T(""), _T(""), false, true, false, Core::NodeId::AnyInterface(), remoteNode, 1024, 1024)
//        {
//        }
//        virtual ~EchoWebSocketClient()
//        {
//        }
//
//    private:
//        virtual void Received(string& text)
//        {
//            printf(_T("Received: %s\n"), text.c_str());
//        }
//        virtual void Send(const string& text)
//        {
//            printf(_T("Send: %s\n"), text.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[5] Open - OK\n"));
//            }
//            else {
//                printf(_T("[5] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//    };
//
//    // -----------------------------------------------------------------------------------------------
//    // TEST SET 6
//    // ----------
//    // WebSocket Echo service (JSON), send back what we got.
//    // -----------------------------------------------------------------------------------------------
//    class JSONWebSocketServer : public Core::StreamJSONType<Web::WebSocketServerType<Core::SocketStream>, JSONObjectFactory&> {
//    private:
//        typedef Core::StreamJSONType<Web::WebSocketServerType<Core::SocketStream>, WPEFramework::TestSystem::JSONObjectFactory&> BaseClass;
//
//    private:
//        JSONWebSocketServer(const JSONWebSocketServer&);
//        JSONWebSocketServer& operator=(const JSONWebSocketServer&);
//
//    public:
//        JSONWebSocketServer(const SOCKET& socket, const WPEFramework::Core::NodeId& remoteNode, WPEFramework::Core::SocketServerType<JSONWebSocketServer>&)
//            : BaseClass(5, WPEFramework::TestSystem::JSONObjectFactory::Instance(), false, true, false, socket, remoteNode, 256, 256)
//        {
//        }
//        virtual ~JSONWebSocketServer()
//        {
//        }
//
//    public:
//        virtual void Received(Core::ProxyType<Core::JSON::IElement>& jsonObject)
//        {
//            if (jsonObject.IsValid() == false) {
//                printf("Oops");
//            }
//            else {
//                string textElement;
//                jsonObject->ToString(textElement);
//
//                printf(_T("   Bytes: %d\n"), static_cast<uint32>(textElement.size()));
//                printf(_T("Received: %s\n"), textElement.c_str());
//
//                // As this is the server, send back the Element we received...
//                Submit(jsonObject);
//            }
//        }
//        virtual void Send(Core::ProxyType<Core::JSON::IElement>& jsonObject)
//        {
//            string textElement;
//            jsonObject->ToString(textElement);
//
//            printf(_T("Bytes: %d\n"), static_cast<uint32>(textElement.size()));
//            printf(_T(" Send: %s\n"), textElement.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[6] Open - OK\n"));
//            }
//            else {
//                printf(_T("[6] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//    };
//
//    class JSONWebSocketClient : public Core::StreamJSONType<Web::WebSocketClientType<Core::SocketStream>, WPEFramework::TestSystem::JSONObjectFactory&> {
//    private:
//        typedef Core::StreamJSONType<Web::WebSocketClientType<Core::SocketStream>, WPEFramework::TestSystem::JSONObjectFactory&> BaseClass;
//
//    private:
//        JSONWebSocketClient(const JSONWebSocketClient&);
//        JSONWebSocketClient& operator=(const JSONWebSocketClient&);
//
//    public:
//        JSONWebSocketClient(const Core::NodeId& remoteNode)
//            : BaseClass(5, WPEFramework::TestSystem::JSONObjectFactory::Instance(), _T("/"), _T("echo"), _T(""), _T(""), false, true, false, Core::NodeId::AnyInterface(), remoteNode, 256, 256)
//        {
//        }
//        virtual ~JSONWebSocketClient()
//        {
//        }
//
//    public:
//        virtual void Received(Core::ProxyType<Core::JSON::IElement>& jsonObject)
//        {
//            string textElement;
//            jsonObject->ToString(textElement);
//
//            printf(_T("   Bytes: %d\n"), static_cast<uint32>(textElement.size()));
//            printf(_T("Received: %s\n"), textElement.c_str());
//        }
//        virtual void Send(Core::ProxyType<Core::JSON::IElement>& jsonObject)
//        {
//            string textElement;
//            jsonObject->ToString(textElement);
//
//            printf(_T("Bytes: %d\n"), static_cast<uint32>(textElement.size()));
//            printf(_T(" Send: %s\n"), textElement.c_str());
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[6] Open - OK\n"));
//            }
//            else {
//                printf(_T("[6] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//        virtual bool IsIdle() const
//        {
//            return (true);
//        }
//    };
//    // -----------------------------------------------------------------------------------------------
//    // STRESS TEST SET 8
//    // ----------
//    // Direct connection for transfering text in an echo fashion.
//    // -----------------------------------------------------------------------------------------------
//    class StressTextConnector : public Core::StreamTextType<Core::SocketStream, Core::TerminatorCarriageReturn> {
//    private:
//        typedef Core::StreamTextType<Core::SocketStream, Core::TerminatorCarriageReturn> BaseClass;
//
//        StressTextConnector();
//        StressTextConnector(const StressTextConnector& copy);
//        StressTextConnector& operator=(const StressTextConnector&);
//
//    public:
//        StressTextConnector(const WPEFramework::Core::NodeId& remoteNode)
//            : BaseClass(false, Core::NodeId::AnyInterface(), remoteNode, 1024, 1024)
//            , _serverSocket(false)
//            , _pending(0)
//            , _errors(0)
//            , _sizeReceived(0)
//            , _sizeSend(0)
//            , _messagesReceived(0)
//            , _messagesSend(0)
//        {
//        }
//        StressTextConnector(const SOCKET& connector, const Core::NodeId& remoteId, Core::SocketServerType<StressTextConnector>&)
//            : BaseClass(false, connector, remoteId, 1024, 1024)
//            , _serverSocket(true)
//            , _pending(0)
//            , _errors(0)
//            , _sizeReceived(0)
//            , _sizeSend(0)
//            , _messagesReceived(0)
//            , _messagesSend(0)
//        {
//        }
//        virtual ~StressTextConnector()
//        {
//            Close(WPEFramework::Core::infinite);
//        }
//
//    public:
//        inline uint32 Pending() const
//        {
//            return (_pending);
//        }
//        inline uint32 Errors() const
//        {
//            return (_errors);
//        }
//        inline uint32 MinSize() const
//        {
//            return (_factory.MinSize());
//        }
//        inline uint32 MaxSize() const
//        {
//            return (_factory.MaxSize());
//        }
//        inline uint32 MessagesReceived() const
//        {
//            return (_messagesReceived);
//        }
//        inline uint32 MessagesSend() const
//        {
//            return (_messagesSend);
//        }
//        inline uint32 SizeReceived() const
//        {
//            return (static_cast<uint32>(_sizeReceived / (1024 * 1024)));
//        }
//        inline uint32 SizeSend() const
//        {
//            return (static_cast<uint32>(_sizeSend / (1024 * 1024)));
//        }
//
//        void Fire(const uint32 runCount)
//        {
//            uint32 current = 0;
//
//            printf("Firing: %d requests\n", runCount);
//
//            while (current < runCount) {
//                string data = _factory.GenerateData();
//
//                Submit(data);
//
//                current++;
//            }
//        }
//        virtual void Received(string& text)
//        {
//            _messagesReceived++;
//            _sizeReceived += text.length();
//
//            if ((_messagesReceived % 1000) == 0) {
//                printf("Received: %d on %s\n", _messagesReceived, LocalId().c_str());
//            }
//
//            if (_factory.VerifyData(text) == false) {
//                Core::InterlockedIncrement(_errors);
//                printf("Failure: %s", text.c_str());
//            }
//            else if (_serverSocket == true) {
//                Submit(_factory.GenerateData());
//            }
//
//            if (_serverSocket == false) {
//                Core::InterlockedDecrement(_pending);
//            }
//        }
//        virtual void Send(const string& text)
//        {
//            _messagesSend++;
//            _sizeSend += text.length();
//
//            if ((_messagesSend % 1000) == 0) {
//                printf("Send: %d on %s\n", _messagesSend, LocalId().c_str());
//            }
//
//            if (_serverSocket == false) {
//                Core::InterlockedIncrement(_pending);
//            }
//        }
//        virtual void StateChange()
//        {
//            printf(_T("State change: "));
//            if (IsOpen()) {
//                printf(_T("[8] Open - OK\n"));
//            }
//            else {
//                printf(_T("[8] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
//            }
//        }
//
//    private:
//        bool _serverSocket;
//        uint32 _pending;
//        uint32 _errors;
//        uint64 _sizeReceived;
//        uint64 _sizeSend;
//        uint32 _messagesReceived;
//        uint32 _messagesSend;
//        StressTextFactory _factory;
//    };
//
//    // -----------------------------------------------------------------------------------------------
//    // FILE SERVER TEST SET 9
//    // ----------
//    // Offer a file client to the server
//    // -----------------------------------------------------------------------------------------------
//    class FileClientConnector : public Web::ClientTransferType<Core::SocketStream, Web::SignedFileBodyType<Crypto::SHA256HMAC> > {
//    private:
//        typedef Web::ClientTransferType<Core::SocketStream, Web::SignedFileBodyType<Crypto::SHA256HMAC> > BaseClass;
//
//        FileClientConnector(const FileClientConnector&);
//        FileClientConnector& operator=(const FileClientConnector&);
//
//    public:
//        FileClientConnector()
//            : BaseClass(string(_T("MySecretTestKey")), false, Core::NodeId::AnyInterface(), Core::NodeId::AnyInterface(), 1024, 1024)
//        {
//        }
//        virtual ~FileClientConnector()
//        {
//        }
//
//    private:
//        virtual bool Setup(const Core::URL& remote)
//        {
//            bool result = false;
//
//            if (remote.Host().IsSet() == true) {
//                uint16 portNumber(remote.Port().IsSet() ? remote.Port().Value() : 80);
//
//                BaseClass::Link().RemoteNode(Core::NodeId(remote.Host().Value().Text().c_str(), portNumber));
//
//                result = true;
//            }
//            return (result);
//        }
//        virtual void Transfered(const uint32 result, Core::File& file)
//        {
//            printf(_T("Transfered file with result: %d\n"), result);
//            file.Close();
//        }
//    };
//
//    // -----------------------------------------------------------------------------------------------
//    // FILE SERVER TEST SET 9
//    // ----------
//    // Offer a file server
//    // -----------------------------------------------------------------------------------------------
//    class FileServerConnector : public Web::ServerTransferType<Core::SocketStream, Web::SignedFileBodyType<Crypto::SHA256HMAC> > {
//    private:
//        inline static const string& PathPrefix()
//        {
//            static const string result(_T("F:/windows/TestArea/"));
//
//            return (result);
//        }
//
//        inline static const string& HashKey()
//        {
//            static const string result(_T("MySecretTestKey"));
//
//            return (result);
//        }
//
//    private:
//        FileServerConnector(const FileServerConnector&);
//        FileServerConnector& operator=(const FileServerConnector&);
//
//    public:
//        FileServerConnector(const SOCKET& connector, const Core::NodeId& remoteId, Core::SocketServerType<FileServerConnector>&)
//            : Web::ServerTransferType<Core::SocketStream, Web::SignedFileBodyType<Crypto::SHA256HMAC> >(PathPrefix(), HashKey(), false, connector, remoteId, 1024, 1024)
//        {
//        }
//        virtual ~FileServerConnector()
//        {
//        }
//
//    public:
//        virtual string Authorize(const Web::Request& request)
//        {
//            if (request.Verb == Web::Request::HTTP_PUT) {
//                printf("Upload request received for [%s]\n", request.Path.c_str());
//            }
//            else {
//                printf("Download request received for [%s]\n", request.Path.c_str());
//            }
//
//            return (EMPTY_STRING);
//        }
//    };
//
//    template <typename CONNECTOR>
//    class StressInstanceType : public Core::Thread {
//    private:
//        StressInstanceType();
//        StressInstanceType(const StressInstanceType<CONNECTOR>&);
//        StressInstanceType<CONNECTOR>& operator=(const StressInstanceType<CONNECTOR>&);
//
//    public:
//        StressInstanceType(const WPEFramework::Core::NodeId& remoteNode)
//            : Thread(_T("StressTestEngine"))
//            , _connector(remoteNode)
//            , _fireSet(0)
//        {
//            _connector.Open(0);
//            TRACE_L1("StressEngine instance: %d", Core::Thread::Id());
//        }
//        ~StressInstanceType()
//        {
//            _connector.Close(WPEFramework::Core::infinite);
//        }
//
//    public:
//        inline uint32 Pending() const
//        {
//            return (_connector.Pending());
//        }
//        inline uint32 Errors() const
//        {
//            return (_connector.Errors());
//        }
//        inline uint32 MinSize() const
//        {
//            return (_connector.MinSize());
//        }
//        inline uint32 MaxSize() const
//        {
//            return (_connector.MaxSize());
//        }
//        inline uint32 MessagesReceived() const
//        {
//            return (_connector.MessagesReceived());
//        }
//        inline uint32 MessagesSend() const
//        {
//            return (_connector.MessagesSend());
//        }
//        inline uint32 SizeReceived() const
//        {
//            return (_connector.SizeReceived());
//        }
//        inline uint32 SizeSend() const
//        {
//            return (_connector.SizeSend());
//        }
//        inline void Fire(const uint32 setSize)
//        {
//            if (IsRunning() == false) {
//                _fireSet = setSize;
//                Run();
//            }
//        }
//        inline void DirectFire(const uint32 setSize)
//        {
//            _connector.Fire(setSize);
//        }
//
//    private:
//        virtual uint32 Worker()
//        {
//            _connector.Fire(_fireSet);
//
//            Block();
//
//            return (WPEFramework::Core::infinite);
//        }
//
//    private:
//        CONNECTOR _connector;
//        uint32 _fireSet;
//    };

} // namespace EchoProtocol
} // namespace TestSupport
} // namespace WPEFramework

#endif //__ECHOPROTOCOL_H