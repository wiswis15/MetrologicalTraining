#ifndef __MQC_HANDLEINIT_H
#define __MQC_HANDLEINIT_H

#include "InitMessage.h"

namespace WPEFramework {
namespace MQC {
    class HandleInit : public HandlerType<InitMessage> {
    private:
        HandleInit(const HandleInit&);
        HandleInit& operator=(const HandleInit&);

    public:
        HandleInit();
        virtual ~HandleInit();

        virtual void Procedure(InitMessage& message);
    };

    class HandleFloat : public HandlerType<FloatMessage> {
    private:
        HandleFloat(const HandleFloat&);
        HandleFloat& operator=(const HandleFloat&);

    public:
        HandleFloat();
        virtual ~HandleFloat();

        virtual void Procedure(FloatMessage& message);
    };
}
}

#endif /* __MQC_HANDLEINIT_H */
