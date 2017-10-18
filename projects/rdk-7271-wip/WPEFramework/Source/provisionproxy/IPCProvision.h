#ifndef __IPC_PROVISION_H__
#define __IPC_PROVISION_H__

#include <core/core.h>

using namespace WPEFramework;

namespace IPC { namespace Provisioning {

class KeyValue
{
        private:
                KeyValue (const KeyValue&);
                KeyValue& operator= (const KeyValue&);

	public:
                KeyValue () : _maxSize(0), _filledSize(0), _buffer(nullptr)
                {
                }
		KeyValue(const uint32 size) : _maxSize (size), _filledSize (0), _buffer(new uint8 [_maxSize])
                {
                }
		KeyValue(const uint32 size, const uint8 stream[]) : _maxSize (size+1), _filledSize (size), _buffer(new uint8 [size+1])
                {
                        ::memcpy(_buffer, stream, size);
                }
		~KeyValue()
                {
                }

	public:
                inline void Clear()
                {
                        if (_buffer != NULL)
                        {
                            ::memset(_buffer, 0,  _maxSize);
                        }
                        _filledSize = 0;
                }
                inline uint32 Length () const
                {
                        return (_filledSize);
                }
		inline const uint8* Frame () const
                {
                        return (_buffer);
                }
                inline void Frame (const uint32 length, const uint8 stream[])
                {
                        Deserialize(stream, length, 0);
                }

                inline uint16 Serialize (uint8 stream[], const uint16 maxLength, const uint32 offset) const
                {
                        uint16 result = 0;

                        if (offset < _filledSize)
                        {
                                result = ((_filledSize - offset) <= maxLength ? (_filledSize - offset) : maxLength);
                                ::memcpy(stream, &(_buffer[offset]), result);
                        }
                        return (result);
                }
                inline uint16 Deserialize (const uint8 stream[], const uint16 maxLength,const uint32 offset)
                {
                        uint16 result = maxLength;

                        if ((offset + maxLength) > _maxSize)
                        {
                                _maxSize = 2 * (offset + maxLength);
                                uint8* buffer  = new uint8[_maxSize];

                                // We need to expand. Current size does not fit..
                                if (_buffer != NULL)
                                {
                                        ::memcpy (buffer, _buffer, _filledSize);
                                        delete _buffer;
                                }

                                _buffer = buffer;
                        }

                        _filledSize = offset + maxLength;

                        ::memcpy(&(_buffer[offset]), stream, maxLength);

                        return (result);
                }

	private:
		uint32 _maxSize;
                uint32 _filledSize;
                uint8* _buffer;
};

typedef Core::IPCMessageType<1, Core::IPC::Text<32>, IPC::Provisioning::KeyValue>	DrmIdData;
typedef Core::IPCMessageType<2, Core::IPC::Void, Core::IPC::Text<32> >			DeviceIdData;

} } // namespace IPC::Provisioning

#endif // __IPC_PROVISION_H__
