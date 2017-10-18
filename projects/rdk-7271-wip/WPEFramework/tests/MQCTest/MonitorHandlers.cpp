#include "MonitorHandlers.h"

#include <iostream>

#include "SharedBools.h"

namespace WPEFramework {
namespace Horizon {
    ProcessStartedHandler::ProcessStartedHandler()
    {
    }

    ProcessStartedHandler::~ProcessStartedHandler()
    {
    }

    /* virtual */ void ProcessStartedHandler::Procedure(ProcessStartedMessage& message)
    {
        /*
    TRACE_L1("Received CreateWebViewMessage, Sequence: %d", message.Request().Sequence());
    TRACE_L1("Config line: %s", message.Inbound().Configuration().c_str());

    TCHAR data[256];
    sprintf (data, "x=%d,y=%d,z=%d,width=%d,height=%d", message.Inbound().X(), message.Inbound().Y(), message.Inbound().Z(), message.Inbound().Width(), message.Inbound().Height());
    TRACE_L1("Received data: %s", data);
    */

        std::cout << "ProcessStartedHandler::Procedure: " << message.Inbound().Id() << std::endl;

        g_ReceivedProcessStarted = true;
    }
}
}
