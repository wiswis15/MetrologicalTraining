#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../../generics/generics.h"
#include "../../websocket/websocket.h"

using namespace WPEFramework;

namespace WebSocketTest {

	class Message : public Core::JSON::Container {
	private:
		Message(const Message&) = delete;
		Message& operator= (const Message&) = delete;

	public:
		Message() :
			Core::JSON::Container(),
			EventType(),
			Event() {
			Add(_T("eventType"), &EventType);
			Add(_T("event"), &Event);
		}
		~Message() {
		}

	public:
		Core::JSON::String EventType;
		Core::JSON::String Event;
	};

	class Factory : public Core::ProxyPoolType<Message> {
	private:
		Factory() = delete;
		Factory(const Factory&) = delete;
		Factory& operator= (const Factory&) = delete;

	public:
		Factory(const uint32 number) : Core::ProxyPoolType<Message>(number) {
		}
		virtual ~Factory() {
		}

	public:
		Core::ProxyType<Core::JSON::IElement> Element(const string&) {
			return (Core::proxy_cast<Core::JSON::IElement>(Core::ProxyPoolType<Message>::Element()));
		}
	};

	class SocketServerLink : public Core::StreamJSONType< Web::WebSocketServerType<Core::SocketStream>, Factory& > {
	private:
		typedef Core::StreamJSONType< Web::WebSocketServerType<Core::SocketStream>, Factory& > BaseClass;

	private:
		SocketServerLink(const SocketServerLink&) = delete;
		SocketServerLink& operator=(const SocketServerLink&) = delete;

	public:
		SocketServerLink(const SOCKET& socket, const WPEFramework::Core::NodeId& remoteNode, Core::SocketServerType<SocketServerLink>*)
			: BaseClass(2, _objectFactory, false, false, false, socket, remoteNode, 512, 512)
			, _objectFactory(1) {
		}
		virtual ~SocketServerLink() {
		}

	public:
		virtual bool IsIdle() const { return (true); }
		virtual void StateChange() {

			if (IsOpen() == true) {
				TRACE_L1(_T("WIS observer channel is open. %d"), __LINE__);
			}
			else if (Link().Link().HasError() == true) {
				Close(0);
			}
		}

		bool IsAttached() const {
			return (IsOpen());
		}
		virtual void Received(Core::ProxyType<Core::JSON::IElement>& jsonObject) {
			string textElement;
			jsonObject->ToString(textElement);


			Core::ProxyType<Message> message(Core::proxy_cast<Message>(jsonObject));

			printf("We got it, whatver it is :-)\n\n");
		}
		virtual void Send(Core::ProxyType<Core::JSON::IElement>& jsonObject) {
			string textElement;
			jsonObject->ToString(textElement);

		}
	private:
		Factory _objectFactory;
	};

	class SocketClientLink : public Core::StreamJSONType<Web::WebSocketClientType<Core::SocketStream>, Factory&> {
	private:
		typedef Core::StreamJSONType<Web::WebSocketClientType<Core::SocketStream>, Factory&> BaseClass;

	private:
		SocketClientLink(const SocketClientLink&) = delete;
		SocketClientLink& operator=(const SocketClientLink&) = delete;

	public:
		SocketClientLink(const Core::NodeId& remoteNode)
			: BaseClass(5, _objectFactory, _T(""), _T(""), _T(""), _T(""), false, true, false, remoteNode.AnyInterface(), remoteNode, 256, 256)
			, _objectFactory(2)
		{
		}
		virtual ~SocketClientLink()
		{
		}

	public:
		virtual void Received(Core::ProxyType<Core::JSON::IElement>& jsonObject)
		{
			string textElement;
			jsonObject->ToString(textElement);

			printf(_T("   Bytes: %d\n"), static_cast<uint32>(textElement.size()));
			printf(_T("Received: %s\n"), textElement.c_str());
		}
		virtual void Send(Core::ProxyType<Core::JSON::IElement>& jsonObject)
		{
			string textElement;
			jsonObject->ToString(textElement);

			printf(_T("Bytes: %d\n"), static_cast<uint32>(textElement.size()));
			printf(_T(" Send: %s\n"), textElement.c_str());
		}
		virtual void StateChange()
		{
			printf(_T("State change: "));
			if (IsOpen()) {
				printf(_T("[6] Open - OK\n"));
			}
			else {
				printf(_T("[6] Closed - %s\n"), (IsSuspended() ? _T("SUSPENDED") : _T("OK")));
			}
		}
		virtual bool IsIdle() const
		{
			return (true);
		}

	private:
		Factory _objectFactory;
	};

	bool ParseOptions(int argc, char** argv, Core::NodeId& listenNode, bool& server)
{
    int index = 1;
    bool domain = false;
    bool showHelp = false;
    const char* hostName = NULL;

	server = true;

    while ((index < argc) && (showHelp == false)) {
        if (strcmp(argv[index], "-domain") == 0) {
            showHelp |= (hostName != NULL);
            domain = true;
        }
        else if (strcmp(argv[index], "-connect") == 0) {
            if (((index + 1) < argc) && (argv[index + 1][0] != '-')) {
                showHelp |= (hostName != NULL);
                hostName = argv[index + 1];
				server = false;
                index++;
            }
        }
        else {
            showHelp = true;
        }
        index++;
    }

    if (showHelp == false) {
        if (domain == true) {
            listenNode = Core::NodeId("/tmp/testserver0");
        }
        else if ((hostName == nullptr) || (hostName[0] == '\0')) {
            listenNode = Core::NodeId("0.0.0.0", 5555);
        }
        else {
            listenNode = Core::NodeId(hostName, 5555);
        }
    }

    return (showHelp);
}

} // namespace

int main(int argc, char** argv)
{
    // If there are no arguments, we act as a server, otherwise we will be the client :-)
    uint32 error;
    int element;
	bool server;
    Core::NodeId listenNode;

    if (WebSocketTest::ParseOptions(argc, argv, listenNode, server) == true) {
        printf("WebSocketTest [-domain] [-connect listenNode]\n");
        Core::Singleton::Dispose();
        exit(0);
    }

    printf("Creating the listening channel for text WebSocket links. [%s]\n", listenNode.QualifiedName().c_str());

    Core::SocketServerType<WebSocketTest::SocketServerLink> _webSocketServer(listenNode);
	WebSocketTest::SocketClientLink _webSocketClient(listenNode);
	Core::ProxyType<WebSocketTest::Message> _message (Core::ProxyType<WebSocketTest::Message>::Create());

	if (server == true)
	{
		// Start listening
		_webSocketServer.Open(0);
	}

    do {
		printf("\n>");
        element = toupper(getchar());

        switch (element) {
		case '0': {
			if (server == true) {
				printf("Server is always listening.\n");
			}
			else if (_webSocketClient.IsOpen() == true) {
				printf("Closing Client connection.\n");
				_webSocketClient.Close(Core::infinite);
			}
			else {
				printf("Opening Client connection.\n");
				_webSocketClient.Open(Core::infinite);
			}
			break;
		}
        case '1': {
            // Send out a ping..
            auto iterator(_webSocketServer.Clients());

            while (iterator.Next() == true) {
                printf("Pinging a websocket. Id: [%s]...\n", iterator.Client()->Link().LocalId());
                iterator.Client()->Link().Ping();
            }
            break;
        }
        case '2': {


            break;
        }
        case '3': {

            break;
        }
        }

    } while (element != 'Q');

    printf("Leaving app.\n");

    error = _webSocketServer.Close(1000); // Wait for 1 second
    Core::Singleton::Dispose();

    return (0);
}
