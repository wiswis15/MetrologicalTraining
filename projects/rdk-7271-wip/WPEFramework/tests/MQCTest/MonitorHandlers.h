#ifndef __HORIZON_MONITORHANDLERS_H
#define __HORIZON_MONITORHANDLERS_H

#include "MonitorMessages.h"

namespace WPEFramework {
namespace Horizon {
    class ProcessStartedHandler : public MQC::HandlerType<ProcessStartedMessage> {
    private:
        ProcessStartedHandler(const ProcessStartedHandler&);
        ProcessStartedHandler& operator=(const ProcessStartedHandler&);

    public:
        ProcessStartedHandler();
        virtual ~ProcessStartedHandler();

        virtual void Procedure(ProcessStartedMessage& message);
    };
}
}

#endif /* __HORIZON_MONITORHANDLERS_H */
