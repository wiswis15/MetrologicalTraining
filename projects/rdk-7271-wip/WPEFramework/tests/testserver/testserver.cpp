#include <generics/generics.h>
#include <websocket/websocket.h>
#include <devices/devices.h>

#include "EchoProtocol.h"

namespace WPEFramework {
namespace TestSystem {
    /* static */ Core::ProxyPoolType<Web::TextBody> WebServer::_textBodyFactory(5);
    /* static */ Core::ProxyPoolType<Web::Response> WebServer::_responseFactory(5);
    /* static */ Core::ProxyPoolType<Web::Request> JSONWebServer::_requestFactory(5);
    /* static */ Core::ProxyPoolType<Web::JSONBodyType<DataContainer::Command> > JSONWebServer::_commandBodyFactory(5);

#ifdef __LINUX__
    const TCHAR ConfigFile[] = _T("/home/administrator/WPEFrameworkConfig.json");
    const TCHAR SerialUSBPort[] = _T("/dev/ttyACM0");
#endif

#ifdef __WIN32__
    const TCHAR ConfigFile[] = _T("C:\\Projects/WPEFrameworkConfig.json");
    const TCHAR SerialUSBPort[] = _T("COM7");
#endif

    extern "C" {

    class ConsoleOptions : public WPEFramework::Core::Options {
    public:
        ConsoleOptions(int argumentCount, TCHAR* arguments[])
            : WPEFramework::Core::Options(argumentCount, arguments, _T("v:hsp:d"))
            , LogLevel()
            , SSL(false)
            , Port(9999)
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

#ifdef __WIN32__
    int _tmain(int argc, _TCHAR* argv[])
#else
    int main(int argc, char** argv)
#endif
    {
        ConsoleOptions options(argc, argv);

        fprintf(stderr, "TestServer\n"
                        "(C) Copyright 2014 Pierre Wielders <pierre@wielders.net>\n");

        if (options.RequestUsage()) {
            fprintf(stderr, "Usage: TestServer "
                            "<server address> [-p=<p>]\n");
            return (1);
        }

        const TCHAR* address = options.Command();

        if (address == NULL) {
            address = _T("0.0.0.0");
        }

        WPEFramework::DataContainer::Command jsonTestCommand;

        jsonTestCommand.Identifier = 1234;
        jsonTestCommand.Name = _T("TestingToFile");
        jsonTestCommand.Params.Duration = 456;
        jsonTestCommand.Params.Speed = 345;

        {
            Core::File newFile(ConfigFile);
            newFile.Append();

            jsonTestCommand.ToFile(newFile);
        }

        jsonTestCommand.Clear();

        {
            WPEFramework::DataContainer::Command number2;
            WPEFramework::DataContainer::Command number3;
            Core::File readFile(ConfigFile);
            readFile.Open(true);

            number3.Clear();
            jsonTestCommand.FromFile(readFile);
            number2.FromFile(readFile);
            number3.FromFile(readFile);
        }

        // ------------------------------------------------------------------------------------
        // Factories
        // ------------------------------------------------------------------------------------
        // Factories for JSON objects..
        WPEFramework::TestSystem::JSONObjectFactory::Instance().CreateFactory<WPEFramework::Core::JSON::LabelType<WPEFramework::DataContainer::Command> >(5);

        //-------------------------------------------------------------------------------------------
        // [1] BASE TEXT SOCKETS !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "[1] Building Base Text Socket\n");

        // Build a SocketServer
        WPEFramework::Core::SocketServerType<TextConnector> _textSocketServer(WPEFramework::Core::NodeId(address, 12341));

        // Start lisening !!!!
        _textSocketServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // [2] BASE JSON SOCKETS !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "[2] Building Base JSON Socket\n");

        // Build a SocketServer
        WPEFramework::Core::SocketServerType<JSONConnector> _JSONSocketServer(WPEFramework::Core::NodeId(address, 12342));

        // Start lisening !!!!
        _JSONSocketServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // [3] HTTPSOCKETS !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "[3] Building HTTP Sockets\n");

        Core::SocketServerType<WebServer> _webServer(WPEFramework::Core::NodeId(address, 12343));

        // Start listening
        _webServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // [4] JSON HTTP SOCKETS !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "[4] Building JSON HTTP Sockets\n");

        Core::SocketServerType<JSONWebServer> _jsonWebServer(WPEFramework::Core::NodeId(address, 12344));

        // Start listening
        _jsonWebServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // [5] TEXT WEBSOCKET SERVER !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "[5] Building Web Sockets for Text\n");

        Core::SocketServerType<EchoWebSocketServer> _textWebSocketServer(WPEFramework::Core::NodeId(address, 12345));

        // Start listening
        _textWebSocketServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // [6] JSON WEBSOCKET SERVER !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "[6] Building Web Sockets for JSON\n");

        Core::SocketServerType<JSONWebSocketServer> _JSONWebSocketServer(WPEFramework::Core::NodeId(address, 12346));

        // Start listening
        _JSONWebSocketServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // [8] STRESS TEXT SOCKETS !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "[1] Building Stress Text Socket\n");

        // Build a SocketServer
        WPEFramework::Core::SocketServerType<StressTextConnector> _stressTextSocketServer(WPEFramework::Core::NodeId(address, 12348));

        // Start lisening !!!!
        _stressTextSocketServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // [9] FILE SERVER SOCKET !!!!!
        //-------------------------------------------------------------------------------------------
        WPEFramework::Core::SocketServerType<FileServerConnector> _fileServer(WPEFramework::Core::NodeId(address, 12349));

        _fileServer.Open(0);

        //-------------------------------------------------------------------------------------------
        // USB to SERIAL bridge  !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "Building a USB/Serial bridge\n");

        WPEFramework::SerialUSB::PIC18F2550 usbToSerial(SerialUSBPort);

        //-------------------------------------------------------------------------------------------
        // Other testing stuff !!!!!
        //-------------------------------------------------------------------------------------------
        fprintf(stderr, "Start reading a directory\n");

        WPEFramework::Core::Directory pluginDirectory(_T("C:\\Projects\\applications\\Debug"), _T("*.dll"));

        while (pluginDirectory.Next() == true) {
            WPEFramework::Core::File file(pluginDirectory.Current().c_str());

            if (file.Exists()) {
                if (file.IsDirectory()) {
                    printf("Directory Found: %s\n", file.Name().c_str());
                }
                else {
                    // Check if we can load it..
                    WPEFramework::Core::Library newLibrary(file.Name().c_str());

                    if (newLibrary.IsLoaded()) {
                        void* handle = newLibrary.LoadFunction(_T("GetServices"));

                        if (handle != NULL) {
                            printf("Got the method\n");
                        }
                        else {
                            printf("No required method\n");
                        }
                    }
                    printf("File Found: %s\n", file.Name().c_str());
                }
            }
        };

        char keyPress;
        bool flagState = false;

        do {
            keyPress = toupper(getchar());

            switch (keyPress) {
            case '1': {
                for (uint32 teller = 0; teller < 100000; teller++) {
                    flagState = !flagState;
                    usbToSerial.SetOutput(WPEFramework::SerialUSB::PIC18F2550::PIN_B4, flagState);
                    string version = usbToSerial.Version();

                    if (version != _T("<ZMD:iCP12v1>")) {
                        if (version.empty() == false) {
                            printf("\nIncorect version!!! [%s]\n", version.c_str());
                        }
                        else {
                            printf("\nIncorect version!!! <empty>\n");
                        }
                    }

                    if ((teller % 5000) == 0) {
                        printf("\nreached: %d\n", teller);
                    }
                }
            }
            case 'T': {
                // Toggle a LED.
                flagState = !flagState;
                usbToSerial.SetOutput(WPEFramework::SerialUSB::PIC18F2550::PIN_B4, flagState);
                break;
            }
            case 'V': {
                // Toggle a LED.
                string version = usbToSerial.Version();
                printf("Protocol/Device version:%s\n", version.c_str());
                break;
            }
            case 'S': {
                WPEFramework::Core::FactoryType<WPEFramework::Core::JSON::IElement, string>::Iterator index(WPEFramework::TestSystem::JSONObjectFactory::Instance().Factories());

                printf("Current stats:\n");

                while (index.Next() == true) {
                    printf("Factory name:     %s\n", index.Label().c_str());
                    printf("Created elements: %d\n", index.CreatedElements());
                    printf("Queued elements:  %d\n", index.QueuedElements());
                    printf("Queue max size:   %d\n\n", index.CurrentQueueSize());
                }

                break;
            }
            default:
                printf("you pressed %c, only q and s have a meaning here\n", keyPress);
                break;
            }

        } while (keyPress != 'Q');

        printf("\nLeaving the main App !!!\n");
        return 0;
    }

    } // extern "C"
}
} // namespace WPEFramework::TestSystem
