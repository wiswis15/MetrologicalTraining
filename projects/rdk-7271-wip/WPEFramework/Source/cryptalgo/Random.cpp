#include "Random.h"

#ifdef __LINUX__
#include <arpa/inet.h>
#endif // __LINUX__

#ifdef __WIN32__
#include "Winsock2.h"
#endif // __WIN32__

namespace WPEFramework {
namespace Crypto {
    // --------------------------------------------------------------------------------------------
    // RANDOM functionality
    // --------------------------------------------------------------------------------------------
    void Reseed()
    {
        srand(static_cast<unsigned int>(time(NULL)));
    }

    void Random(uint8& value)
    {
#if RAND_MAX >= 0xFF
        srand(static_cast<unsigned int>(time(NULL)));

#ifdef __WIN32__
        value = static_cast<uint8>(rand() & 0xFF);
#endif // __WIN32__
#ifdef __LINUX__
        value = static_cast<uint8>(random() & 0xFF);
#endif // __LINUX__
#else
#error "Can not create random functionality"
#endif
    }

    void Random(uint16& value)
    {
#if RAND_MAX >= 0xFFFF
#ifdef __WIN32__
        value = static_cast<uint16>(rand() & 0xFFFF);
#endif // __WIN32__
#ifdef __LINUX__
        value = static_cast<uint16>(random() & 0xFFFF);
#endif // __LINUX__
#elif RAND_MAX >= 0xFF
#ifdef __WIN32__
        uint8 hsb = static_cast<uint8>(rand() & 0xFF);
        uint8 lsb = static_cast<uint8>(rand() & 0xFF);
#endif // __WIN32__
#ifdef __LINUX__
        uint8 hsb = static_cast<uint8>(random() & 0xFF);
        uint8 lsb = static_cast<uint8>(random() & 0xFF);
#endif // __LINUX__
        value = (value << 8) | lsb;
#else
#error "Can not create random functionality"
#endif
    }

    void Random(uint32& value)
    {
#if RAND_MAX >= 0xFFFFFFFF
#ifdef __WIN32__
        value = static_cast<uint32>(rand() & 0xFFFFFFFF);
#endif // __WIN32__
#ifdef __LINUX__
        value = static_cast<uint32>(random() & 0xFFFFFFFF);
#endif // __LINUX__
#elif RAND_MAX >= 0xFFFF
#ifdef __WIN32__
        uint16 hsw = static_cast<uint16>(rand() & 0xFFFF);
        uint16 lsw = static_cast<uint16>(rand() & 0xFFFF);
#endif // __WIN32__
#ifdef __LINUX__
        uint16 hsw = static_cast<uint16>(random() & 0xFFFF);
        uint16 lsw = static_cast<uint16>(random() & 0xFFFF);
#endif // __LINUX__
        value = (hsw << 16) | lsw;
#elif RAND_MAX >= 0xFF
#ifdef __WIN32__
        uint8 hsb1 = static_cast<uint8>(rand() & 0xFF);
        uint8 lsb1 = static_cast<uint8>(rand() & 0xFF);
        uint8 hsb2 = static_cast<uint8>(rand() & 0xFF);
        uint8 lsb2 = static_cast<uint8>(rand() & 0xFF);
#endif // __WIN32__
#ifdef __LINUX__
        uint8 hsb1 = static_cast<uint8>(random() & 0xFF);
        uint8 lsb1 = static_cast<uint8>(random() & 0xFF);
        uint8 hsb2 = static_cast<uint8>(random() & 0xFF);
        uint8 lsb2 = static_cast<uint8>(random() & 0xFF);
#endif // __LINUX__
        value = (hsb1 << 24) | (lsb1 < 16) | (hsb2 << 8) | lsb2;
#else
#error "Can not create random functionality"
#endif
    }

    void Random(uint64& value)
    {
#if RAND_MAX >= 0xFFFFFFFF
#ifdef __WIN32__
        value = static_cast<uint32>(rand() & 0xFFFFFFFF);
        value = static_cast<uint32>(rand() & 0xFFFFFFFF);
#endif // __WIN32__
#ifdef __LINUX__
        value = static_cast<uint32>(random() & 0xFFFFFFFF);
        value = static_cast<uint32>(random() & 0xFFFFFFFF);
#endif // __LINUX__
#elif RAND_MAX >= 0xFFFF
#ifdef __WIN32__
        uint16 hsw1 = static_cast<uint16>(rand() & 0xFFFF);
        uint16 lsw1 = static_cast<uint16>(rand() & 0xFFFF);
        uint16 hsw2 = static_cast<uint16>(rand() & 0xFFFF);
        uint16 lsw2 = static_cast<uint16>(rand() & 0xFFFF);
#endif // __WIN32__
#ifdef __LINUX__
        uint16 hsw1 = static_cast<uint16>(random() & 0xFFFF);
        uint16 lsw1 = static_cast<uint16>(random() & 0xFFFF);
        uint16 hsw2 = static_cast<uint16>(random() & 0xFFFF);
        uint16 lsw2 = static_cast<uint16>(random() & 0xFFFF);
#endif // __LINUX__
        value = (hsw1 << 16) | lsw1;
        value = (value << 32) | (hsw2 << 16) | lsw2;
#elif RAND_MAX >= 0xFF
#ifdef __WIN32__
        uint8 hsb1 = static_cast<uint8>(rand() & 0xFF);
        uint8 lsb1 = static_cast<uint8>(rand() & 0xFF);
        uint8 hsb2 = static_cast<uint8>(rand() & 0xFF);
        uint8 lsb2 = static_cast<uint8>(rand() & 0xFF);
        uint8 hsb3 = static_cast<uint8>(rand() & 0xFF);
        uint8 lsb3 = static_cast<uint8>(rand() & 0xFF);
        uint8 hsb4 = static_cast<uint8>(rand() & 0xFF);
        uint8 lsb4 = static_cast<uint8>(rand() & 0xFF);
#endif // __WIN32__
#ifdef __LINUX__
        uint8 hsb1 = static_cast<uint8>(random() & 0xFF);
        uint8 lsb1 = static_cast<uint8>(random() & 0xFF);
        uint8 hsb2 = static_cast<uint8>(random() & 0xFF);
        uint8 lsb2 = static_cast<uint8>(random() & 0xFF);
        uint8 hsb3 = static_cast<uint8>(random() & 0xFF);
        uint8 lsb3 = static_cast<uint8>(random() & 0xFF);
        uint8 hsb4 = static_cast<uint8>(random() & 0xFF);
        uint8 lsb4 = static_cast<uint8>(random() & 0xFF);
#endif // __LINUX__
        value = (hsb1 << 24) | (lsb1 < 16) | (hsb2 << 8) | lsb2;
        value = (value << 32) | (hsb3 << 24) | (lsb3 < 16) | (hsb4 << 8) | lsb4;
#else
#error "Can not create random functionality"
#endif
    }
}
} // namespace WPEFramework::Crypto
