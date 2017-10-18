#include "InitMessage.h"

namespace WPEFramework {
namespace MQC {
    const TCHAR InitCommand[] = _T("init");

    InitRequest::InitRequest()
        : Request()
        , Width(0)
        , Height(0)
    {
    }

    /* virtual */ InitRequest::~InitRequest()
    {
    }

    /* virtual */ void InitRequest::Serialize(Frame::Writer& write) const
    {
        Request::Serialize(write);
        write.Number(Width);
        write.Number(Height);
    }

    /* virtual */ void InitRequest::Deserialize(const Frame::Reader& read)
    {
        Request::Deserialize(read);
        Width = read.Number<uint32>();
        Height = read.Number<uint32>();
    }

    const TCHAR FloatCommand[] = _T("float");

    FloatRequest::FloatRequest()
        : Request()
        , _value(0)
    {
    }

    /* virtual */ FloatRequest::~FloatRequest()
    {
    }

    /* virtual */ void FloatRequest::Serialize(Frame::Writer& write) const
    {
        Request::Serialize(write);
        write.Number(_value);
    }

    /* virtual */ void FloatRequest::Deserialize(const Frame::Reader& read)
    {
        Request::Deserialize(read);
        _value = read.Number<float>();
    }
}
}
