#include "WebClient.h"

const char* g_HostName = "test.integraal.info";
const char* g_HostPath = "/CustomJavascript/geoloc01.html";
const int g_PortNumber = 80;

int main()
{
    WPEFramework::WebClient webConnector(WPEFramework::Core::NodeId(g_HostName, g_PortNumber), g_HostPath);
    webConnector.Open(0);

    // Sleep ten seconds to finish page load.
    // TODO: semaphore
    sleep(10);

    return 0;
}
