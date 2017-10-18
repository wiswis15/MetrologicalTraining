#ifndef __HASHSTREAM_H
#define __HASHSTREAM_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "Module.h"
#include "Hash.h"
#include "HMAC.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----
namespace WPEFramework {
namespace Crypto {
    struct IHashStream {
        virtual ~IHashStream(){};

        virtual EnumHashType Type() const = 0;
        virtual void Reset() = 0;
        virtual uint8* Result() = 0;
        virtual uint8 Length() const = 0;
        virtual void Input(const uint8 block, const uint16 length) = 0;
    };

    template <typename HASHALGORITHM, const enum EnumHashType TYPE>
    class HashStreamType {
        private :
            HashStreamType(const HashStreamType<HASHALGORITHM, TYPE>&);
        HashStreamType<HASHALGORITHM, TYPE>&
        operator=(const HashStreamType<HASHALGORITHM, TYPE>&);

        public :
            // For Hash streaming
            HashStreamType();

        // For HMAC streaming
        HashStreamType(const Core::TextFragment& key);

        public :
            virtual void Reset(){
                _hash.Reset(); }
    virtual uint8* Result()
    {
        return (_hash.Result());
    }
    virtual uint8 Length() const
    {
        return (HASHALGORITHM::Length());
    }
    virtual void Input(const uint8 block[], const uint16 length)
    {
        _hash.Input(block, length);
    }
    virtual EnumHashType Type() const
    {
        return (TYPE);
    }

private:
    HASHALGORITHM _hash;
};
}
}

#endif // __HASHSTREAM_H
