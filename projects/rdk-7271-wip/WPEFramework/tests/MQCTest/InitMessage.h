#ifndef __MQC_INITMESSAGE_H
#define __MQC_INITMESSAGE_H

#include "../../mqc/mqc.h"

namespace WPEFramework {
namespace MQC {
    class InitRequest : public Request {
    public:
        InitRequest();
        virtual ~InitRequest();

        int GetWidth() const
        {
            return Width;
        }
        int GetHeight() const
        {
            return Height;
        }

        void SetWidth(int width)
        {
            Width = width;
        }
        void SetHeight(int height)
        {
            Height = height;
        }

    private:
        virtual void Serialize(Frame::Writer& write) const;
        virtual void Deserialize(const Frame::Reader& read);

    private:
        int Width;
        int Height;
    };

    extern const TCHAR InitCommand[];

    typedef WPEFramework::MQC::MessageType<InitCommand, 10, InitRequest, Message> InitMessage;

    class FloatRequest : public Request {
    public:
        FloatRequest();
        virtual ~FloatRequest();

        float Value() const
        {
            return _value;
        }
        void Value(const float value)
        {
            _value = value;
        }

    private:
        virtual void Serialize(Frame::Writer& write) const;
        virtual void Deserialize(const Frame::Reader& read);

    private:
        float _value;
    };

    extern const TCHAR FloatCommand[];

    typedef WPEFramework::MQC::MessageType<FloatCommand, 0xAA55, FloatRequest> FloatMessage;
}
}

#endif /* __MQC_INITMESSAGE_H */
