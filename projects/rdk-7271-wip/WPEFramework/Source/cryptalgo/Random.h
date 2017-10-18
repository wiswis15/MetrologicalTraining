#ifndef __RANDOM_H
#define __RANDOM_H

// ---- Include system wide include files ----
#include <time.h>

// ---- Include local include files ----
#include "Module.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----
namespace WPEFramework {
namespace Crypto {
    extern EXTERNAL void Reseed();
    extern EXTERNAL void Random(uint8& value);
    extern EXTERNAL void Random(uint16& value);
    extern EXTERNAL void Random(uint32& value);
    extern EXTERNAL void Random(uint64& value);

    inline void Random(sint8& value)
    {
        Random(reinterpret_cast<uint8&>(value));
    }
    inline void Random(sint16& value)
    {
        Random(reinterpret_cast<uint16&>(value));
    }
    inline void Random(sint32& value)
    {
        Random(reinterpret_cast<uint32&>(value));
    }
    inline void Random(sint64& value)
    {
        Random(reinterpret_cast<uint64&>(value));
    }

    template <typename DESTTYPE>
    void Random(DESTTYPE& value, const DESTTYPE minimum, const DESTTYPE maximum)
    {
        Crypto::Random(value);

        value = ((value % (maximum - minimum)) + minimum);
    }
}
}

#endif // __RANDOM_H
