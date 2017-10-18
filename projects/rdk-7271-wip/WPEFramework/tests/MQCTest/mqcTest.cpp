#include "../../generics/generics.h"

#include "HandleInit.h"
#include "HandleAsync.h"
#include "MonitorMessages.h"
#include "ProxyMessages.h"
#include "ProxyHandlers.h"

#include "LaunchTest.h"

using namespace WPEFramework;

static void PrintUsage(const char progName[])
{
    std::cout << "Usage: " << progName << " [-server|-client|-time <runtime in ms>]" << std::endl;
}

#include "ProcessLoop.h"

int main(int argc, const char** argv)
{
    TCHAR target[256];
    uint32 sleepTime = 900000;
    uint32 runMode = 4;
    const TCHAR* sourceQueue = "/webkitprocess";
    const TCHAR* targetQueue = "/webkitmonitor";
    const TCHAR* appMode = "Server";

    printf("Starting up the MQC Test client...\n");

    if (argc > 1) {
        if (!strcmp(argv[1], "-server")) {
            runMode = 1;
            sourceQueue = "/testserver";
            targetQueue = "/testclient";
        }
        else if (!strcmp(argv[1], "-float")) {
            runMode = 1;
            sourceQueue = "/testclient";
            targetQueue = "/testserver";
            appMode = "Client";
        }
        else if (!strcmp(argv[1], "-client")) {
            // TODO: get this one nicely in flow below.
            RunLaunchTest();
            std::cout << "Done with Launch test" << std::endl;
            return 0;
        }
        else if ((!strcmp(argv[1], "-time")) && (argc > 2)) {
            sleepTime = atoi(argv[2]);
        }
        else if ((!strcmp(argv[1], "-fake")) && (argc > 2)) {
            runMode = 3;
            sourceQueue = "/bsqueue";
            sleepTime = 2 * 60 * 60 * 1000;
        }
        else if ((!strcmp(argv[1], "-name")) && (argc > 2)) {
            runMode = 2;
            target[0] = '/';
            strcpy(&(target[1]), argv[2]);
            sourceQueue = target;
            targetQueue = "/testserver";
        }
        else {
            PrintUsage(argv[0]);
        }
    }

    {
        std::cout << "Opening up the PostOffice: " << sourceQueue << "..." << std::endl
                  << std::endl;
        MQC::Postbox postbox;

        uint32 result = postbox.Open(sourceQueue);

        ProcessLoop activeHandler(postbox);

        if (result != Core::ERROR_NONE) {
            std::cout << "Opening up the message queue failed. Error: " << result << std::endl;
        }
        else if (runMode == 1) {
            std::cout << "Entering the " << appMode << " loop." << std::endl;

            char keyPress;

            do {

                keyPress = toupper(getchar());

                switch (keyPress) {
                case 'F': {
                    static float nextValue = 1.1;
                    nextValue += 0.7;

                    MQC::FloatMessage floatMessage;
                    floatMessage.Inbound().Value(nextValue);
                    postbox.Relay(targetQueue, floatMessage, WPEFramework::Core::infinite);
                    break;
                }
                case 'Q': {
                    break;
                }
                default: {
                    std::cout << "Press Q to quit the server." << std::endl;
                    break;
                }
                }

            } while (keyPress != 'Q');
        }
        else if (runMode == 2) {
            std::cout << "Entering the Client loop." << std::endl;

            uint32 value = 0;

            MQC::InitMessage initMessage;
            MQC::AsyncMessage asyncMessage;

            std::cout << "Entering the Client loop." << std::endl;

            do {

                SleepMs(10);

                initMessage.Inbound().SetWidth(value);
                initMessage.Inbound().SetHeight(value);

                postbox.Relay(targetQueue, initMessage, WPEFramework::Core::infinite);

                asyncMessage.Inbound().SetValue(value);

                postbox.Relay(targetQueue, asyncMessage, WPEFramework::Core::infinite);

                ++value;

            } while (true);
        }
        else if (runMode == 3) {
            Horizon::CreateWebViewHandler createWebViewHandler;

            postbox.Register<WPEFramework::Horizon::CreateWebViewMessage>(createWebViewHandler, 2);

            // This is the HTML Proxy Testing Mode..
            std::cout << "HTML Proxy Testing mode." << std::endl;

            Horizon::ProcessStartedMessage processStartedMessage;

            processStartedMessage.Inbound().Id(WPEFramework::Core::ProcessInfo().Id());

            std::cout << "Send the message to: " << targetQueue << std::endl;
            postbox.Relay(targetQueue, processStartedMessage, WPEFramework::Core::infinite);

            std::cout << "Sleep for " << sleepTime << " ms." << std::endl;
            SleepMs(sleepTime);

            postbox.Unregister(createWebViewHandler);

            std::cout << "Done testing." << std::endl;
        }
        else if (runMode == 4) {
            std::cout << "HTML Proxy Testing mode (load handler)." << std::endl;

            Horizon::LoadHandler loadHandler;

            postbox.Register<WPEFramework::Horizon::LoadMessage>(loadHandler, 2);

            std::cout << "Entering the Server loop." << std::endl;

            char keyPress;

            do {
                keyPress = toupper(getchar());

                if (keyPress != 'Q') {
                    std::cout << "Press Q to quit the server." << std::endl;
                }
            } while (keyPress != 'Q');
        }
        else if (runMode == 5) {
            uint32 value = 0;

            Horizon::LoadMessage loadMessage;

            std::cout << "Entering the Client loop (load handler)." << std::endl;

            do {
                SleepMs(10);

                loadMessage.Inbound().ApplicationId(value);
                loadMessage.Inbound().URL("http://www.veryveryveryverylongurl.com/can/we/make/it/even/longer?yes=true");

                postbox.Relay(targetQueue, loadMessage, WPEFramework::Core::infinite);

                ++value;

            } while (true);
        }

        postbox.Close();
    }

    std::cout << "Dispose testing." << std::endl;
    WPEFramework::Core::Singleton::Dispose();
    std::cout << "Disposed." << std::endl;

    return 0;
}
