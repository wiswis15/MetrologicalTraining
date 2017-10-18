#include "HandleAsync.h"

namespace WPEFramework {
namespace MQC {
    HandleAsync::HandleAsync()
    {
    }

    HandleAsync::~HandleAsync()
    {
    }

    // TODO: use "Core::ProxyType"
    /* virtual */ void HandleAsync::Procedure(AsyncMessage& message)
    {
        std::cout << "In HandleAsync::Procedure, value: " << message.Inbound().GetValue() << std::endl;
    }
}
}
