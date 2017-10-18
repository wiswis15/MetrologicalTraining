#ifndef __IMEMORY_H
#define __IMEMORY_H

#include "Module.h"

namespace WPEFramework {
namespace Exchange {

    // This interface allows for retrieval of memory usage specific to the implementor
    // of the interface
    struct IMemory : virtual public Core::IUnknown {
        enum { ID = 0x00000052 };
        virtual ~IMemory() {}

        virtual uint64 Resident() const = 0;
        virtual uint64 Allocated() const = 0;
        virtual uint64 Shared() const = 0;
        virtual uint8 Processes() const = 0;
        virtual const bool IsOperational() const = 0;
    };
}
}

#endif // __IMEMORY_H
