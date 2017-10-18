#include <generics/generics.h>
#include <websocket/websocket.h>

#include "WebInspectorProtocol.h"

namespace WPEFramework {
// ------------------------------------------------------------------------------------
// Classname:   JSONObjectFactory
// Description: This singleton class allows all the JSON objects, flowwing through the
//              websocket, to be instantiated. The JSON objects are reference counted
//              JSON objects. If the reference count reaches 0, the object is not
//              destructed but returned to a pool. A next request/creation for such a
//              JSON object, will first check the pool. Only if the pool is empty,
//              a new JSON object is created.
// ------------------------------------------------------------------------------------
class JSONObjectFactory : public Core::JSON::Factory {
private:
    inline JSONObjectFactory()
        : Core::JSON::Factory()
    {
        // Define all the JSON objects that we can create
        CreateFactory<Core::JSON::LabelType<WebInspector::Request> >(5);
        CreateFactory<Core::JSON::LabelType<WebInspector::Notification> >(5);
    }
    JSONObjectFactory(const JSONObjectFactory&);
    JSONObjectFactory& operator=(const JSONObjectFactory&);

public:
    static JSONObjectFactory& Instance()
    {
        static JSONObjectFactory _singleton;

        return (_singleton);
    }
    inline ~JSONObjectFactory()
    {
    }
};

// ------------------------------------------------------------------------------------
// Classname:   WebInspectorLink
// Description: This class is the WebSocketLink for JSON objects. It allows to submit
//              JSON objects to be passed to the otherside and it will publish JSON
//              objects received. Creation of the JSON object received, is realised by
//              the JSONObjectFactory. See class above.
//              The JSON objects used are reference counted!!
// ------------------------------------------------------------------------------------
class WebInspectorLink : public Core::StreamJSONType<Web::WebSocketClientType<Core::SocketStream>, JSONObjectFactory&> {
private:
    typedef Core::StreamJSONType<Web::WebSocketClientType<Core::SocketStream>, JSONObjectFactory&> BaseClass;

private:
    WebInspectorLink(const WebInspectorLink&);
    WebInspectorLink& operator=(const WebInspectorLink&);

public:
    WebInspectorLink(const Core::NodeId& remoteNode)
        : BaseClass(5, JSONObjectFactory::Instance(), _T("/"), _T("webinspector"), false, false, false, remoteNode, 1024, 1024)
    {
    }
    virtual ~WebInspectorLink()
    {
    }

public:
    virtual void Received(Core::ProxyType<Core::JSON::IElement>& jsonObject)
    {
    }
    virtual void Send(Core::ProxyType<Core::JSON::IElement>& jsonObject)
    {
    }
    virtual void StateChange()
    {
        printf(_T("State change: "));
        if (IsOpen()) {
            printf(_T("[6] Open - %s\n"), (HasError() ? _T("ERROR") : _T("OK")));
        }
        else {
            printf(_T("[6] Closed - %s\n"), (HasError() ? _T("ERROR") : _T("OK")));
        }
    }
};

// ------------------------------------------------------------------------------------
// Classname:   ConsoleOptions
// Description: All command line options passed to this application will be analysed
//              and translated in the following class object.
// ------------------------------------------------------------------------------------
class ConsoleOptions : public WPEFramework::Core::Options {
public:
    ConsoleOptions(int argumentCount, TCHAR* arguments[])
        : WPEFramework::Core::Options(argumentCount, arguments, _T("v:hsp:d"))
        , LogLevel()
        , SSL(false)
        , Port(80)
        , Version(1)
    {
        Parse();
    }
    ~ConsoleOptions()
    {
    }

public:
    WPEFramework::Core::OptionalType<uint8> LogLevel;
    bool SSL;
    uint16 Port;
    uint16 Version;

private:
    virtual void Option(const TCHAR option, const TCHAR* argument)
    {
        switch (option) {
        case 'd':
            LogLevel = atoi(argument);
            break;
        case 's':
            SSL = true;
            break;
        case 'p':
            Port = atoi(argument);
            break;
        case 'v':
            Version = atoi(argument);
            break;
        case 'h':
        default:
            RequestUsage(true);
            break;
        }
    }
};

extern "C" {

#ifdef __WIN32__
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char** argv)
#endif
{
    ConsoleOptions options(argc, argv);

    fprintf(stderr, "webinspector remote client\n"
                    "(C) Copyright 2014 Pierre Wielders <pierre@wielders.net>\n"
                    "licensed under LGPL2.1\n");

    if ((options.RequestUsage() == true) || (argc < 2)) {
        fprintf(stderr, "Usage: libwebsockets-test-client "
                        "<server address> [--port=<p>]\n");
        return (1);
    }

    // Create the websocket connector to WebInspector
    WebInspectorLink webinspectorLink(WPEFramework::Core::NodeId(options.Command(), options.Port));
    JSONObjectFactory& factory(JSONObjectFactory::Instance());

    /* create a client websocket using dumb increment protocol */
    char keyPress;
    bool linkOpen = false;
    uint32 requestId = 0;

    do {
        keyPress = toupper(getchar());

        switch (keyPress) {
        case 'O': {
            webinspectorLink.Open();
            break;
        }
        case 'C': {
            webinspectorLink.Open();
            break;
        }
        case 'R': {
            // Get a request from the factory to send some data to webinspector...
            Core::ProxyType<WebInspector::Request> data(factory.Get<WebInspector::Request>());
        }
        }

    } while (keyPress != 'Q');

    return 0;
}

} // extern "C"

} // namespace WPEFramework