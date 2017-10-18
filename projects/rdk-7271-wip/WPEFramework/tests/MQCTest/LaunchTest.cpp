#include "LaunchTest.h"

#include "../../generics/generics.h"

#include "HandleInit.h"
#include "HandleAsync.h"
#include "MonitorMessages.h"
#include "ProxyMessages.h"
#include "ProxyHandlers.h"
#include "MonitorHandlers.h"

#include "ProcessLoop.h"

#include "SharedBools.h"

volatile bool g_ReceivedProcessStarted = false;

void RunLaunchTest(/*WPEFramework::MQC::Postbox& postbox*/)
{
    std::cout << "Entering the Client loop for Launch test." << std::endl;

    const char* monitorQueueName = "/webkitmonitor";
    const char* proxyQueueName = "/webkitproxy";
    const char* targetQueueName = "/webkitprocess";

    WPEFramework::Horizon::ProcessStartedHandler processStartedHandler;

    WPEFramework::MQC::Postbox monitorPostbox;
    monitorPostbox.Register<WPEFramework::Horizon::ProcessStartedMessage>(processStartedHandler, 2);
    monitorPostbox.Open(monitorQueueName);
    ProcessLoop activeMonitorHandler(monitorPostbox);

    WPEFramework::MQC::Postbox proxyPostbox;
    proxyPostbox.Open(proxyQueueName);
    ProcessLoop activeProxyHandler(proxyPostbox);

    // TODO: semaphore
    while (!g_ReceivedProcessStarted)
        ;

    WPEFramework::Horizon::CreateWebViewMessage createWebViewMessage;
    createWebViewMessage.Inbound().X(1);
    createWebViewMessage.Inbound().Y(2);
    createWebViewMessage.Inbound().Z(3);
    createWebViewMessage.Inbound().Width(1280);
    createWebViewMessage.Inbound().Height(720);
    createWebViewMessage.Inbound().Configuration("");

    std::cout << "About to send CreateWebView message..." << std::endl;
    proxyPostbox.Relay(targetQueueName, createWebViewMessage, WPEFramework::Core::infinite);
    std::cout << "Sent CreateWebView message, handle: " << createWebViewMessage.Outbound().Handle() << std::endl;

    WPEFramework::Horizon::LoadMessage loadMessage;
    loadMessage.Inbound().URL("http://www.google.com");

    std::cout << "About to send Load message..." << std::endl;
    proxyPostbox.Relay(targetQueueName, loadMessage, WPEFramework::Core::infinite);
    std::cout << "Sent Load message" << std::endl;

    WPEFramework::Horizon::HideMessage hideMessage;
    std::cout << "About to send Hide message..." << std::endl;
    proxyPostbox.Relay(targetQueueName, hideMessage, WPEFramework::Core::infinite);
    std::cout << "Sent Hide message" << std::endl;

    WPEFramework::Horizon::SaveCookiesMessage saveCookiesMessage;
    std::cout << "About to send SaveCookies message..." << std::endl;
    proxyPostbox.Relay(targetQueueName, saveCookiesMessage, WPEFramework::Core::infinite);
    std::cout << "Sent SaveCookies message" << std::endl;

    //WPEFramework::MQC::InitMessage initMessage;

    //const int width = 1280;
    //const int height = 720;

    //initMessage.Inbound().SetWidth(width);
    //initMessage.Inbound().SetHeight(height);

    //postbox.Relay(targetQueueName, initMessage, WPEFramework::Core::infinite);
}
