#pragma once

#include <core/core.h>
#include <com/com.h>

// TODO: better not to use namespace in header
using namespace WPEFramework;

namespace media {

extern Core::CriticalSection _systemLock;

class DataExchange: public WPEFramework::Core::SharedBuffer {
private:
	DataExchange() = delete;
	DataExchange(const DataExchange&) = delete;
	DataExchange& operator=(const DataExchange&) = delete;

private:
	struct Administration {
		uint8_t IVLength;
		uint8_t IV[16];
		uint16_t SubLength;
		uint8_t Sub[2048];
	};

public:
	DataExchange(const string& name) :
		_busy(false),
		WPEFramework::Core::SharedBuffer(name.c_str())
	{
		TRACE_L1("Constructing buffer client side: %p - %s", this, name.c_str());
	}

/*
	DataExchange(const string& name, const uint32_t bufferSize) :
			WPEFramework::Core::SharedBuffer(name.c_str(), bufferSize,
					sizeof(Administration)), _busy(false) {
		Administration* admin =	reinterpret_cast<Administration*>(AdministrationBuffer());
		// Clear the administration space before using it.
		::memset(admin, 0, sizeof(Administration));
	}
*/
	~DataExchange() {
        if (_busy == true) {
            TRACE_L1("Destructed a DataExchange while still in progress. %p", this);
        }
        TRACE_L1("Destructing buffer client side: %p - %s", this, Name().c_str());
	}

public:
	void SetIV(const uint8_t ivDataLength, const uint8_t ivData[]) {
		Administration* admin =
				reinterpret_cast<Administration*>(AdministrationBuffer());
		ASSERT(ivDataLength <= sizeof(Administration::IV));
		admin->IVLength = (
				ivDataLength > sizeof(Administration::IV) ?
						sizeof(Administration::IV) : ivDataLength);
		::memcpy(admin->IV, ivData, admin->IVLength);
		if (admin->IVLength < sizeof(Administration::IV)) {
			::memset(&(admin->IV[admin->IVLength]), 0,
					(sizeof(Administration::IV) - admin->IVLength));
		}
	}
	void SetSubSampleData(const uint16_t length, const uint8_t* data) {
		Administration* admin =
				reinterpret_cast<Administration*>(AdministrationBuffer());
		admin->SubLength = (
				length > sizeof(Administration::Sub) ?
						sizeof(Administration::Sub) : length);
		if (data != nullptr) {
			::memcpy(admin->Sub, data, admin->SubLength);
		}
	}
	void Write(const uint32_t length, const uint8_t* data) {

		if (WPEFramework::Core::SharedBuffer::Size(length) == true) {
			SetBuffer(0, length, data);
		}
	}
	void Read(const uint32_t length, uint8_t* data) const {
		GetBuffer(0, length, data);
	}
	const uint8_t* IVKey() const {
		const Administration* admin =
				reinterpret_cast<const Administration*>(AdministrationBuffer());
		return (admin->IV);
	}
	const uint8_t IVKeyLength() const {
		const Administration* admin =
				reinterpret_cast<const Administration*>(AdministrationBuffer());
		return (admin->IVLength);
	}
    uint32_t Decrypt(uint8_t* encryptedData, uint32_t encryptedDataLength, const uint8_t* ivData, uint16_t ivDataLength) {
         int ret = 0;

         // This works, because we know that the Audio and the Video streams are fed from
         // the same process, so they will use the same critial section and thus will
         // not interfere with each-other. If Audio and video will be located into two
         // different processes, start using the administartion space to share a lock.
         _systemLock.Lock();

         _busy = true;

         if (RequestProduce(WPEFramework::Core::infinite) == WPEFramework::Core::ERROR_NONE) {

             SetIV(ivDataLength, ivData);
             SetSubSampleData(0, nullptr);
             Write(encryptedDataLength, encryptedData);

             // This will trigger the OpenCDMIServer to decrypt this memory...
             Produced();

             // Now we should wait till it is decrypted, that happens if the Producer, can run again.
             if (RequestProduce(WPEFramework::Core::infinite) == WPEFramework::Core::ERROR_NONE) {

                 // For nowe we just copy the clear data..
                 Read(encryptedDataLength, encryptedData);

                 // And free the lock, for the next production Scenario..
                 Consumed();
             }
         }

         _busy = false;

         _systemLock.Unlock();

         return (ret);
     }


private:
	bool _busy;
};

}
