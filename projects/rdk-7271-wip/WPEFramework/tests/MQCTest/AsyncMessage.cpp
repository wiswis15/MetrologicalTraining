#include "AsyncMessage.h"

namespace WPEFramework {
namespace MQC {
    const TCHAR AsyncCommand[] = _T("async");

    AsyncRequest::AsyncRequest()
        : Request()
        , Value(0)
    {
    }

    AsyncRequest::~AsyncRequest()
    {
    }

    /* virtual */ void AsyncRequest::Serialize(Frame::Writer& write) const
    {
        Request::Serialize(write);

        write.Number(Value);
    }

    /* virtual */ void AsyncRequest::Deserialize(const Frame::Reader& read)
    {
        Request::Deserialize(read);

        Value = read.Number<uint32>();
    }
}
}
