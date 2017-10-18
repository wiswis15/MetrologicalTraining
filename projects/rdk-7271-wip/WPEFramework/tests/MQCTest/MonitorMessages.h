#ifndef __HORIZON_MONITORMESSAGE_H
#define __HORIZON_MONITORMESSAGE_H

#include "../../mqc/mqc.h"

namespace WPEFramework {
namespace Horizon {
    class ProcessStarted : public MQC::Request {
    public:
        ProcessStarted();
        virtual ~ProcessStarted();

        uint32 Id() const
        {
            return (_pid);
        }
        void Id(const uint32 pid)
        {
            _pid = pid;
        }

    private:
        virtual void Serialize(Frame::Writer& write) const;
        virtual void Deserialize(const Frame::Reader& read);

    private:
        uint32 _pid;
    };

    extern const TCHAR ProcessStartedCommand[];

    typedef WPEFramework::MQC::MessageType<ProcessStartedCommand, 1, ProcessStarted> ProcessStartedMessage;
}
}

#endif /* __HORIZON_MONITORMESSAGE_H */
