#pragma once

#include <WPEFramework/core/Portability.h> // IUnknown
#include <WPEFramework/com/IUnknown.h>
#include <WPEFramework/com/com.h>

namespace WPEFramework {
namespace Exchange {
    struct IAdder : virtual public Core::IUnknown {
        enum { ID = 0x80000001 };
        virtual uint32_t GetValue() = 0;
        virtual void Add(uint32_t value) = 0;
        virtual uint32_t GetPid() = 0;
    };
}
}
