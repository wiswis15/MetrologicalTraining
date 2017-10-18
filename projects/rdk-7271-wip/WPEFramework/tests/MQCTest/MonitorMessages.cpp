#include "MonitorMessages.h"

namespace WPEFramework {
namespace Horizon {
    const TCHAR ProcessStartedCommand[] = _T("processStarted");

    ProcessStarted::ProcessStarted()
        : Request()
        , _pid(0)
    {
    }

    /* virtual */ ProcessStarted::~ProcessStarted()
    {
    }

    /* virtual */ void ProcessStarted::Serialize(Frame::Writer& write) const
    {
        Request::Serialize(write);
        write.Number(_pid);
    }

    /* virtual */ void ProcessStarted::Deserialize(const Frame::Reader& read)
    {
        Request::Deserialize(read);
        _pid = read.Number<uint32>();
    }
}
}
