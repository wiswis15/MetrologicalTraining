#ifndef __HORIZON_PROXYHANDLERS_H
#define __HORIZON_PROXYHANDLERS_H

#include "ProxyMessages.h"

namespace WPEFramework {
namespace Horizon {
    class CreateWebViewHandler : public MQC::HandlerType<CreateWebViewMessage> {
    private:
        CreateWebViewHandler(const CreateWebViewHandler&);
        CreateWebViewHandler& operator=(const CreateWebViewHandler&);

    public:
        CreateWebViewHandler();
        virtual ~CreateWebViewHandler();

        virtual void Procedure(CreateWebViewMessage& message);
    };

    class LoadHandler : public MQC::HandlerType<LoadMessage> {
    private:
        LoadHandler(const LoadHandler&);

        LoadHandler& operator=(const LoadHandler&);

    public:
        LoadHandler();

        virtual ~LoadHandler();

        virtual void Procedure(LoadMessage& message);

        inline void Handler()
        {
        }

    private:
    };
}
}

#endif // __HORIZON_PROXYHANDLERS_H
