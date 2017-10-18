#include "ProxyHandlers.h"

#include <iostream>

namespace WPEFramework {
namespace Horizon {
    CreateWebViewHandler::CreateWebViewHandler()
    {
    }

    CreateWebViewHandler::~CreateWebViewHandler()
    {
    }

    /* virtual */ void CreateWebViewHandler::Procedure(CreateWebViewMessage& message)
    {
        TRACE_L1("Received CreateWebViewMessage, Sequence: %d", message.Request().Sequence());
        TRACE_L1("Config line: %s", message.Inbound().Configuration().c_str());

        TCHAR data[256];
        sprintf(data, "x=%d,y=%d,z=%d,width=%d,height=%d", message.Inbound().X(), message.Inbound().Y(), message.Inbound().Z(), message.Inbound().Width(), message.Inbound().Height());
        TRACE_L1("Received data: %s", data);

        message.Outbound().Handle(1000);
    }

    LoadHandler::LoadHandler()
    {
    }

    /* virtual */ LoadHandler::~LoadHandler()
    {
    }

    /* virtual */ void LoadHandler::Procedure(LoadMessage& message)
    {
        std::cerr << "In load handler, data:" << std::endl;
        std::cerr << "App ID: " << message.Inbound().ApplicationId() << std::endl;
        std::cerr << "URL: " << message.Inbound().URL() << std::endl;
    }
}
}
