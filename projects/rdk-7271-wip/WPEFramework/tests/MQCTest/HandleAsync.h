#ifndef __MQC_HANDLEASYNC_H
#define __MQC_HANDLEASYNC_H

#include "AsyncMessage.h"

namespace WPEFramework {
namespace MQC {
    class HandleAsync : public HandlerType<AsyncMessage> {
    private:
        HandleAsync(const HandleAsync&);
        HandleAsync& operator=(const HandleAsync&);

    public:
        HandleAsync();
        virtual ~HandleAsync();

        virtual void Procedure(AsyncMessage& request);
    };
}
}

#endif /* __MQC_HANDLEASYNC_H */
