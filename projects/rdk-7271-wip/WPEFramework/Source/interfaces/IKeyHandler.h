#ifndef __IKEYHANDLER_H__
#define __IKEYHANDLER_H__

#include "Module.h"

namespace WPEFramework {
namespace Exchange {
    struct IKeyHandler : virtual public Core::IUnknown {
        enum { ID = 0x00000050 };

        virtual ~IKeyHandler(){};

        virtual uint32 KeyEvent(const bool pressed, const uint32 code, const string& table) = 0;
    };

    struct IKeyProducer : virtual public Core::IUnknown {
        enum { ID = 0x00000051 };

        virtual ~IKeyProducer(){};

        virtual const TCHAR* Name() const = 0;
        virtual bool Pairing() = 0;
        virtual uint32 Callback(IKeyHandler* callback) = 0;
        virtual uint32 Error() const = 0;
        virtual string MetaData() const = 0;
        virtual void CodeMask(uint32 mask) = 0;
    };
}
}

#endif // __IKEYHANDLER_H__
