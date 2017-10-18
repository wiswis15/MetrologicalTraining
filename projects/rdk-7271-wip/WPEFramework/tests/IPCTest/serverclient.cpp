#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../../generics/generics.h"

using namespace WPEFramework;

namespace Messages {

class Response {
public:
    Response()
        : _result(0)
    {
    }
    Response(const uint32 result)
        : _result(result)
    {
    }
    Response(const Response& copy)
        : _result(copy._result)
    {
    }
    ~Response()
    {
    }

    Response& operator=(const Response& RHS)
    {
        _result = RHS._result;

        return (*this);
    }

public:
    inline uint32 Result() const
    {
        return (_result);
    }

private:
    uint32 _result;
};

class Triplet {
public:
    Triplet()
        : _display(0)
        , _surface(1)
        , _context(2)
    {
    }
    Triplet(const uint16 display, const uint32 surface, const uint64 context)
        : _display(display)
        , _surface(surface)
        , _context(context)
    {
    }
    Triplet(const Triplet& copy)
        : _display(copy._display)
        , _surface(copy._surface)
        , _context(copy._context)
    {
    }
    ~Triplet()
    {
    }

    Triplet& operator=(const Triplet& RHS)
    {
        _display = RHS._display;
        _surface = RHS._surface;
        _context = RHS._context;

        return (*this);
    }

public:
    inline uint16 Display() const
    {
        return (_display);
    }
    inline uint32 Surface() const
    {
        return (_surface);
    }
    inline uint64 Context() const
    {
        return (_context);
    }

private:
    uint16 _display;
    uint32 _surface;
    uint64 _context;
};

typedef Core::IPCMessageType<1, Messages::Triplet, Messages::Response> TripletResponse;
typedef Core::IPCMessageType<2, Messages::Triplet, Core::Void> TripletVoid;
typedef Core::IPCMessageType<3, Core::Void, Messages::Triplet> VoidTriplet;
typedef Core::IPCMessageType<4, Core::IPC::Text<2048>, Core::Void> TextVoid;
}

class HandleTripletResponse : public Core::IPCServerType<Messages::TripletResponse> {
private:
    HandleTripletResponse(const HandleTripletResponse&);
    HandleTripletResponse& operator=(const HandleTripletResponse&);

public:
    HandleTripletResponse()
        : IPCServerType<Messages::TripletResponse>()
    {
    }
    virtual ~HandleTripletResponse()
    {
    }

public:
    // Here comes the actual implementation of the RPC...
    virtual void Procedure(Core::IPCChannel& channel, Core::ProxyType<Messages::TripletResponse>& data)
    {
        uint32 result = data->Parameters().Display() + data->Parameters().Surface() + static_cast<uint32>(data->Parameters().Context());
        data->Response() = Messages::Response(result);

        printf("Remote invoked, received [%d,%d,%d] -> [%d] !!!\n", data->Parameters().Display(), data->Parameters().Surface(), static_cast<uint32>(data->Parameters().Context()), result);

        Core::ProxyType<Core::IIPC> proxyData(Core::proxy_cast<Core::IIPC>(data));
        channel.ReportResponse(proxyData);
    }
};

class HandleTripletVoid : public Core::IPCServerType<Messages::TripletVoid> {
private:
    HandleTripletVoid(const HandleTripletVoid&);
    HandleTripletVoid& operator=(const HandleTripletVoid&);

public:
    HandleTripletVoid()
        : IPCServerType<Messages::TripletVoid>()
    {
    }
    virtual ~HandleTripletVoid()
    {
    }

public:
    // Here comes the actual implementation of the RPC...
    virtual void Procedure(Core::IPCChannel& channel, Core::ProxyType<Messages::TripletVoid>& data)
    {
        printf("Remote invoked, received [%d,%d,%d] -> [void] !!!\n", data->Parameters().Display(), data->Parameters().Surface(), static_cast<uint32>(data->Parameters().Context()));
        Core::ProxyType<Core::IIPC> proxyData(Core::proxy_cast<Core::IIPC>(data));
        channel.ReportResponse(proxyData);
    }
};

class HandleVoidTriplet : public Core::IPCServerType<Messages::VoidTriplet> {
private:
    HandleVoidTriplet(const HandleVoidTriplet&);
    HandleVoidTriplet& operator=(const HandleVoidTriplet&);

public:
    HandleVoidTriplet()
        : IPCServerType<Messages::VoidTriplet>()
    {
    }
    virtual ~HandleVoidTriplet()
    {
    }

public:
    // Here comes the actual implementation of the RPC...
    virtual void Procedure(Core::IPCChannel& channel, Core::ProxyType<Messages::VoidTriplet>& data)
    {
        Messages::Triplet newValue(7, 9, _lastParameter++);

        printf("Remote invoked, received [void] -> [7,9,%d]!!!\n", static_cast<uint32>(newValue.Context()));

        data->Response() = newValue;
        Core::ProxyType<Core::IIPC> proxyData(Core::proxy_cast<Core::IIPC>(data));
        channel.ReportResponse(proxyData);
    }

private:
    static uint64 _lastParameter;
};

class HandleTextVoid : public Core::IPCServerType<Messages::TextVoid> {
private:
    HandleTextVoid(const HandleTextVoid&);
    HandleTextVoid& operator=(const HandleTextVoid&);

public:
    HandleTextVoid()
        : IPCServerType<Messages::TextVoid>()
    {
    }
    virtual ~HandleTextVoid()
    {
    }

public:
    // Here comes the actual implementation of the RPC...
    virtual void Procedure(Core::IPCChannel& channel, Core::ProxyType<Messages::TextVoid>& data)
    {
        printf("Received text [%d]: [%s]!!!\n", data->Parameters().Length(), data->Parameters().Value());

        Core::ProxyType<Core::IIPC> proxyData(Core::proxy_cast<Core::IIPC>(data));
        channel.ReportResponse(proxyData);
    }
};

uint64 HandleVoidTriplet::_lastParameter = 0;

bool ParseOptions(int argc, char** argv, Core::NodeId& continousChannel, Core::NodeId& flashChannel, Core::NodeId& multiChannel, bool& server)
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
                index++;
            }

            server = false;
        }
        else {
            showHelp = true;
        }
        index++;
    }

    if (showHelp == false) {
        if (domain == true) {
            continousChannel = Core::NodeId("/tmp/testserver0");
            flashChannel = Core::NodeId("/tmp/testserver1");
            multiChannel = Core::NodeId("/tmp/testserver2");
        }
        else if (server == true) {
            showHelp |= (hostName != NULL);
            continousChannel = Core::NodeId("0.0.0.0", 5555);
            flashChannel = Core::NodeId("0.0.0.0", 5556);
            multiChannel = Core::NodeId("0.0.0.0", 5557);
        }
        else if (hostName != NULL) {
            continousChannel = Core::NodeId(hostName, 5555);
            flashChannel = Core::NodeId(hostName, 5556);
            multiChannel = Core::NodeId(hostName, 5557);
        }
        else {
            showHelp = true;
        }
    }

    return (showHelp);
}

int main(int argc, char** argv)
{
    // If there are no arguments, we act as a server, otherwise we will be the client :-)
    uint32 error;
    int element;
    int run = 3;
    Core::NodeId continousNode;
    Core::NodeId flashNode;
    Core::NodeId multiNode;
    bool server;

    if (ParseOptions(argc, argv, continousNode, flashNode, multiNode, server) == true) {
        printf("IPCTestApp [-domain] [-connect] [remoteNode]\n");
        Core::Singleton::Dispose();
        exit(0);
    }

    printf("Creating the channels.\n");
    printf("   %s [%s]\n", (server ? "Server:" : "Client:"), continousNode.QualifiedName().c_str());
    printf("   %s [%s]\n", (server ? "Server:" : "Client:"), flashNode.QualifiedName().c_str());
    printf("   %s [%s]\n", (server ? "Server:" : "Client:"), multiNode.QualifiedName().c_str());

    Core::ProxyType<Core::FactoryType<Core::IIPC, uint32> > factory(Core::ProxyType<Core::FactoryType<Core::IIPC, uint32> >::Create());

    factory->CreateFactory<Messages::TripletResponse>(2);
    factory->CreateFactory<Messages::TripletVoid>(2);
    factory->CreateFactory<Messages::VoidTriplet>(2);
    factory->CreateFactory<Messages::TextVoid>(2);

    Core::IPCChannelType<Core::SocketPort, Core::Void>* continousChannel = nullptr;
    Core::IPCChannelType<Core::SocketPort, Core::Void>* flashChannel = nullptr;

    if (server == true) {
        printf("Creating ContinousChannel.\n");
        continousChannel = new Core::IPCChannelClientType<Core::Void, true, false>(continousNode, 32, factory);
        printf("Creating FlashChannel.\n");
        flashChannel = new Core::IPCChannelClientType<Core::Void, true, false>(flashNode, 512, factory);
    }
    else {
        printf("Creating ContinousChannel.\n");
        continousChannel = new Core::IPCChannelClientType<Core::Void, false, false>(continousNode, 32, factory);
        printf("Creating FlashChannel.\n");
        flashChannel = new Core::IPCChannelClientType<Core::Void, false, false>(flashNode, 512, factory);
    }

    Core::IPCChannelServerType<Core::Void, false>* multiChannel = nullptr;
    if (server == true) {
        printf("Creating MultiChannel.\n");
        multiChannel = new Core::IPCChannelServerType<Core::Void, false>(multiNode, 512, factory);
    }

    Core::ProxyType<Core::IIPCServer> handler1(Core::ProxyType<HandleTripletResponse>::Create());
    Core::ProxyType<Core::IIPCServer> handler2(Core::ProxyType<HandleTripletVoid>::Create());
    Core::ProxyType<Core::IIPCServer> handler3(Core::ProxyType<HandleVoidTriplet>::Create());
    Core::ProxyType<Core::IIPCServer> handler4(Core::ProxyType<HandleTextVoid>::Create());

    continousChannel->Register(handler1);
    continousChannel->Register(handler2);
    continousChannel->Register(handler3);
    continousChannel->Register(handler4);

    if (server == true) {
        flashChannel->Register(handler1);
        flashChannel->Register(handler2);
        flashChannel->Register(handler3);
        flashChannel->Register(handler4);

        // The server needs to open up his flashChannel as well :-)
        error = flashChannel->Source().Open(1000); // Wait for 1 Second.
        printf("Flash Channel opening up. Error [%d]\n", error);
    }

    error = continousChannel->Source().Open(1000); // Wait for 1 Second.
    printf("Continous Channel opening up. Error [%d]\n", error);

    if (multiChannel != nullptr) {
        multiChannel->Register(handler1);
        multiChannel->Register(handler2);
        multiChannel->Register(handler3);
        multiChannel->Register(handler4);

        error = multiChannel->Open(1000); // Wait for 1 Second.
        printf("Multi Channel opening up. Error [%d]\n", error);
    }

    do {
        element = toupper(getchar());

        switch (element) {
        case '1': {
            Core::ProxyType<Messages::TripletResponse> rpcData(Core::ProxyType<Messages::TripletResponse>::Create(Messages::Triplet(1, 2, run++)));

            error = continousChannel->Invoke(rpcData, 2000);

            if (error == Core::ERROR_NONE) {
                // Succesfull communication. Check feedback
                printf("Invoked, Result [%d].\n", rpcData->Response().Result());
            }
            else {
                printf("Invoked failed, error [%d].\n", error);
            }

            break;
        }
        case '2': {
            Core::ProxyType<Messages::TripletVoid> rpcData(Core::ProxyType<Messages::TripletVoid>::Create(Messages::Triplet(1, 2, run++)));

            error = continousChannel->Invoke(rpcData, 2000);

            if (error == Core::ERROR_NONE) {
                // Succesfull communication. Check feedback
                printf("Invoked, Result [void].\n");
            }
            else {
                printf("Invoked failed, error [%d].\n", error);
            }

            break;
        }
        case '3': {
            Core::ProxyType<Messages::VoidTriplet> rpcData(Core::ProxyType<Messages::VoidTriplet>::Create());

            error = continousChannel->Invoke(rpcData, 2000);

            if (error == Core::ERROR_NONE) {
                // Succesfull communication. Check feedback
                printf("Invoked, Result [%d,%d,%d].\n", rpcData->Response().Display(), rpcData->Response().Surface(), static_cast<uint32>(rpcData->Response().Context()));
            }
            else {
                printf("Invoked failed, error [%d].\n", error);
            }

            break;
        }
        case '4': {
            if (server == true) {
                printf("No use on flash channels.\n");
                break;
            }

            Core::ProxyType<Messages::TripletResponse> rpcData(Core::ProxyType<Messages::TripletResponse>::Create(Messages::Triplet(1, 2, run++)));

            error = flashChannel->Source().Open(1000); // Wait for 1 Second.

            if (error == Core::ERROR_NONE) {
                error = flashChannel->Invoke(rpcData, 2000);

                uint32 closeError = flashChannel->Source().Close(1000); // Wait for 1 Second

                if (error == Core::ERROR_NONE) {
                    // Succesfull communication. Check feedback
                    printf("Invoked, Result [%d].\n", rpcData->Response().Result());
                }
                else {
                    printf("Invoked failed, error [%d].\n", error);
                }

                if (closeError != Core::ERROR_NONE) {
                    printf("Close failed, error [%d].\n", closeError);
                }
            }
            else {
                printf("Failed opening flash channel: [%d].\n", error);
                flashChannel->Source().Close(Core::infinite);
            }

            break;
        }
        case '5': {
            if (server == true) {
                printf("No use on flash channels.\n");
                break;
            }

            Core::ProxyType<Messages::TripletVoid> rpcData(Core::ProxyType<Messages::TripletVoid>::Create(Messages::Triplet(1, 2, run++)));

            error = flashChannel->Source().Open(1000); // Wait for 1 Second.

            if (error == Core::ERROR_NONE) {
                error = flashChannel->Invoke(rpcData, 2000);

                uint32 closeError = flashChannel->Source().Close(1000); // Wait for 1 Second

                if (error == Core::ERROR_NONE) {
                    // Succesfull communication. Check feedback
                    printf("Invoked, Result [Void].\n");
                }
                else {
                    printf("Invoked failed, error [%d].\n", error);
                }

                if (closeError != Core::ERROR_NONE) {
                    printf("Close failed, error [%d].\n", closeError);
                }
            }
            else {
                printf("Failed opening flash channel: [%d].\n", error);
                flashChannel->Source().Close(Core::infinite);
            }

            break;
        }
        case '6': {
            if (server == true) {
                printf("No use on flash channels.\n");
                break;
            }

            Core::ProxyType<Messages::VoidTriplet> rpcData(Core::ProxyType<Messages::VoidTriplet>::Create());

            error = flashChannel->Source().Open(1000); // Wait for 1 Second.

            if (error == Core::ERROR_NONE) {
                error = flashChannel->Invoke(rpcData, 2000);

                uint32 closeError = flashChannel->Source().Close(1000); // Wait for 1 Second

                if (error == Core::ERROR_NONE) {
                    // Succesfull communication. Check feedback
                    printf("Invoked, Result [%d,%d,%d].\n", rpcData->Response().Display(), rpcData->Response().Surface(), static_cast<uint32>(rpcData->Response().Context()));
                }
                else {
                    printf("Invoked failed, error [%d].\n", error);
                }

                if (closeError != Core::ERROR_NONE) {
                    printf("Close failed, error [%d].\n", closeError);
                }
            }
            else {
                printf("Failed opening flash channel: [%d].\n", error);
                flashChannel->Source().Close(Core::infinite);
            }

            break;
        }
        case '7': {
            string text;

            for (unsigned int teller = 0; teller < 1200; teller++) {
                text += static_cast<char>('0' + (teller % 10));
            }

            Core::ProxyType<Messages::TextVoid> rpcData(Core::ProxyType<Messages::TextVoid>::Create());

            rpcData->Parameters() = text;

            error = continousChannel->Invoke(rpcData, 2000);

            if (error == Core::ERROR_NONE) {
                // Succesfull communication. Check feedback
                printf("Invoked, Result [void].\n");
            }
            else {
                printf("Invoked failed, error [%d].\n", error);
            }

            break;
        }
        case '0': {
            const string remoteId(continousChannel->Source().RemoteId());

            printf("Connected to: %s\n", remoteId.c_str());
            break;
        }
        }

    } while (element != 'Q');

    printf("Leaving app.\n");

    error = continousChannel->Source().Close(1000); // Wait for 1 second
    continousChannel->Unregister(handler1);
    continousChannel->Unregister(handler2);
    continousChannel->Unregister(handler3);
    continousChannel->Unregister(handler4);

    printf("Continous Channel closing down. Error [%d].\n", error);

    if (server == true) {
        error = flashChannel->Source().Close(1000); // Wait for 1 Second
        flashChannel->Unregister(handler1);
        flashChannel->Unregister(handler2);
        flashChannel->Unregister(handler3);
        flashChannel->Unregister(handler4);
        printf("Flash Channel closing down. Error [%d].\n", error);
    }

    if (multiChannel != nullptr) {
        error = multiChannel->Close(1000);
        multiChannel->Unregister(handler1);
        multiChannel->Unregister(handler2);
        multiChannel->Unregister(handler3);
        multiChannel->Unregister(handler4);
        delete multiChannel;
        printf("Multi Channel closing down. Error [%d].\n", error);
    }

    factory->DestroyFactories();
    Core::Singleton::Dispose();

    return (0);
}
