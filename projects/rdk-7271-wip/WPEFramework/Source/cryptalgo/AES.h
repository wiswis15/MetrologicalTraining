#ifndef __AES_H
#define __AES_H

#include "AESImplementation.h"
#include "Module.h"

namespace WPEFramework {
namespace Crypto {

    enum aesType {
		AES_ECB,
        AES_CBC,
        AES_CFB8,
        AES_CFB128,
		AES_OFB
    };

	enum bitLength {
		BITLENGTH_128 = 128,
		BITLENGTH_192 = 192,
		BITLENGTH_256 = 256
	};

	class EXTERNAL AESEncryption {
	private:
		AESEncryption() = delete;
		AESEncryption(const AESEncryption&) = delete;
		AESEncryption& operator=(const AESEncryption&) = delete;

	public:
		AESEncryption(const aesType type);
		~AESEncryption();

	public:
		inline aesType Type() const {
			return (_type);
		}
		inline const uint8* InitialVector() const {
			return (_iv);
		}
		inline void InitialVector(const uint8 iv[16]) {
			_offset = 0;
			::memcpy(_iv, iv, sizeof(_iv));
		}
		uint32 Key(const uint8 length, const uint8 key[]);

		uint32 Encrypt(const uint32 length, const uint8 input[], uint8 output[]);

	private:
		aesType _type;
		mbedtls_aes_context _context;
		uint8 _iv[16];
		size_t _offset;
	};

    class EXTERNAL AESDecryption {
    private:
		AESDecryption() = delete;
		AESDecryption(const AESDecryption&) = delete;
		AESDecryption& operator=(const AESDecryption&) = delete;

    public:
		AESDecryption(const aesType type);
        ~AESDecryption();

	public:
		inline aesType Type() const {
			return (_type);
		}
		inline const uint8* InitialVector() const {
			return (_iv);
		}
		inline void InitialVector(const uint8 iv[16]) {
			_offset = 0;
			if (_iv != iv) {
				::memcpy(_iv, iv, sizeof(_iv));
			}
		}
		uint32 Key(const uint8 length, const uint8 key[]);

		uint32 Decrypt(const uint32 length, const uint8 input[], uint8 output[]);

	private:
		aesType _type;
		mbedtls_aes_context _context;
		uint8 _iv[16];
		size_t _offset;
	};
}
} // namespace Crypto

#endif // __AES_H
