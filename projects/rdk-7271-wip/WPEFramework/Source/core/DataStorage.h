#ifndef __DATASTORAGE_H
#define __DATASTORAGE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "Portability.h"
#include "Sync.h"
#include "Proxy.h"
#include "Singleton.h"

namespace WPEFramework {
namespace Core {
    // ---- Referenced classes and types ----

    // ---- Helper types and constants ----

    // ---- Helper functions ----
    template <const unsigned int BLOCKSIZE>
    class ScopedStorage {
    private:
        // ----------------------------------------------------------------
        // Never, ever allow reference counted objects to be assigned.
        // Create a new object and modify it. If the assignement operator
        // is used, give a compile error.
        // ----------------------------------------------------------------
        ScopedStorage(const ScopedStorage<BLOCKSIZE>& a_Copy) = delete;
        ScopedStorage<BLOCKSIZE>& operator=(const ScopedStorage<BLOCKSIZE>& a_RHS) = delete;

    public:
        ScopedStorage()
        {
        }
        virtual ~ScopedStorage()
        {
        }

    public:
        uint8* Buffer()
        {
            return &(m_Buffer[0]);
        }
        uint32 Size() const
        {
            return (BLOCKSIZE);
        }

    private:
        uint8 m_Buffer[BLOCKSIZE];
    };
}
} // namespace Core

#endif // __DATASTORAGE_H
