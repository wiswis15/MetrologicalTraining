#ifndef __MQC_ASYNCMESSAGE_H
#define __MQC_ASYNCMESSAGE_H

#include "../../mqc/mqc.h"

namespace WPEFramework {
namespace MQC {
    class AsyncRequest : public Request {
    public:
        AsyncRequest();
        virtual ~AsyncRequest();

        int GetValue() const
        {
            return Value;
        }
        void SetValue(int value)
        {
            Value = value;
        }

    private:
        virtual void Serialize(Frame::Writer& write) const;
        virtual void Deserialize(const Frame::Reader& read);

    private:
        int Value;
    };

    extern const TCHAR AsyncCommand[];

    typedef WPEFramework::MQC::MessageType<AsyncCommand, 101, AsyncRequest> AsyncMessage;
}
}

#endif /* __MQC_ASYNCMESSAGE_H */
