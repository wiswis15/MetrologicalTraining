#include "HandleInit.h"

namespace WPEFramework {
namespace MQC {
    HandleInit::HandleInit()
    {
    }

    HandleInit::~HandleInit()
    {
    }

    /* virtual */ void HandleInit::Procedure(InitMessage& message)
    {
        std::cout << "In HandleInitMessage::Procedure, width: " << message.Inbound().GetWidth() << std::endl;
    }

    HandleFloat::HandleFloat()
    {
    }

    HandleFloat::~HandleFloat()
    {
    }

    /* virtual */ void HandleFloat::Procedure(FloatMessage& message)
    {
        std::cout << "In HandleFloatMessage::Procedure, width: " << message.Inbound().Value() << std::endl;
    }
}
}
