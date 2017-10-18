#ifndef __GENERICS_FRAME_H
#define __GENERICS_FRAME_H

#include "Module.h"

namespace WPEFramework {
namespace Core {

    template <const uint16 BLOCKSIZE>
    class FrameType {
    private:
        template <const uint16 STARTSIZE>
        class AllocatorType {
        private:
            AllocatorType<STARTSIZE> operator=(const AllocatorType<STARTSIZE>&) = delete;

        public:
            AllocatorType()
                : _bufferSize(STARTSIZE)
                , _data(new uint8[_bufferSize])
            {
                static_assert(STARTSIZE != 0, "This method can only be called if you specify an initial blocksize");
            }
            AllocatorType(const AllocatorType<STARTSIZE>& copy) 
                : _bufferSize(copy._bufferSize)
                , _data (STARTSIZE == 0 ? copy._data : new uint8[_bufferSize]) {

                if (STARTSIZE != 0) {
                    ::memcpy (_data, copy._data, _bufferSize);
                }
            }
            AllocatorType(uint8 buffer[], const uint32 length)
                : _bufferSize(length)
                , _data(buffer)
            {
                static_assert(STARTSIZE == 0, "This method can only be called if you specify an initial blocksize of 0");
            }
            ~AllocatorType()
            {
                if ((STARTSIZE != 0) && (_data != nullptr)) {
                    delete _data;
                }
            }

        public:
            inline uint8& operator[](const uint16 index)
            {
                ASSERT(_data != nullptr);
                ASSERT(index < _bufferSize);
                return (_data[index]);
            }
            inline const uint8& operator[](const uint16 index) const
            {
                ASSERT(_data != nullptr);
                ASSERT(index < _bufferSize);
                return (_data[index]);
            }
            inline void Allocate(uint32 requiredSize)
            {
		RealAllocate<STARTSIZE>(requiredSize, TemplateIntToType<STARTSIZE>());
            }

		private:
			template<const uint16 NONZEROSIZE>
			inline void RealAllocate(const uint32 requiredSize, const TemplateIntToType<NONZEROSIZE>& /* For compile time diffrentiation */) {
				if (requiredSize > _bufferSize) {

					_bufferSize = static_cast<uint16>(((requiredSize / STARTSIZE) + 1) * STARTSIZE);

					// oops we need to "reallocate".
					_data = reinterpret_cast<uint8*>(::realloc(_data, _bufferSize));
				}
			}
			inline void RealAllocate(const uint32 requiredSize, const TemplateIntToType<0>& /* For compile time diffrentiation */) {
				ASSERT(requiredSize <= _bufferSize);
			}

        private:
            uint16 _bufferSize;
            uint8* _data;
        };

    private:
        FrameType& operator=(const FrameType&);

    public:
        class Reader {
        private:
            Reader& operator=(const Reader&);

        public:
            Reader()
                : _offset(0)
                , _container(nullptr)
            {
            }
            Reader(const FrameType& data, const uint16 offset)
                : _offset(offset)
                , _container(&data)
            {
            }
            Reader(const Reader& copy)
                : _offset(copy._offset)
                , _container(copy._container)
            {
            }
            ~Reader()
            {
            }

        public:
            inline bool HasData() const
            {
                return ((_container != nullptr) && (_offset < _container->Size()));
            }
			inline uint16 Length() const {
				return (_container == nullptr ? 0 : _container->Size() - _offset);
			}
			template <typename TYPENAME>
			TYPENAME LockBuffer(const uint8*& buffer) const
			{
				TYPENAME result;

				ASSERT(_container != nullptr);

				_offset += _container->GetNumber<TYPENAME>(_offset, result);
				buffer = &(_container->operator[](_offset));

				return (result);
			}
			template <typename TYPENAME>
			void UnlockBuffer(TYPENAME length) const
			{
				ASSERT(_container != nullptr);

				_offset += length;
			}
			template <typename TYPENAME>
            TYPENAME Buffer(const TYPENAME maxLength, uint8 buffer[]) const
            {
                uint16 result;

                ASSERT(_container != nullptr);

                result   = _container->GetBuffer<TYPENAME>(_offset, maxLength, buffer);
                _offset += result;

                return (static_cast<TYPENAME>(result - sizeof (TYPENAME)));
            }
			void Copy(const uint16 length, uint8 buffer[]) const
			{
				ASSERT(_container != nullptr);

				_offset += _container->Copy(_offset, length, buffer);
			}
			template <typename TYPENAME>
            TYPENAME Number() const
            {
                TYPENAME result;

                ASSERT(_container != nullptr);

                _offset += _container->GetNumber<TYPENAME>(_offset, result);

                return (result);
            }
            bool Boolean() const
            {
                bool result;

                ASSERT(_container != nullptr);

                _offset += _container->GetBoolean(_offset, result);

                return (result);
            }
            string Text() const
            {
                string result;

                ASSERT(_container != nullptr);

                _offset += _container->GetText(_offset, result);

                return (result);
            }
            string NullTerminatedText() const
            {
                string result;

                ASSERT(_container != nullptr);

                _offset += _container->GetNullTerminatedText(_offset, result);

                return (result);
            }
#ifdef __DEBUG__
            void Dump() const
            {
                _container->Dump(_offset);
            }
#endif

        private:
            mutable uint16 _offset;
            const FrameType* _container;
        };
        class Writer {
        private:
            Writer& operator=(const Writer&);

        public:
            Writer()
                : _offset(0)
                , _container(nullptr)
            {
            }
            // TODO: should we make offset 0 by default?
            Writer(FrameType& data, const uint16 offset)
                : _offset(offset)
                , _container(&data)
            {
            }
            Writer(const Writer& copy)
                : _offset(copy._offset)
                , _container(copy._container)
            {
            }
            ~Writer()
            {
            }

        public:
			inline uint16 Offset() const {
				return (_offset);
			}
            template <typename TYPENAME>
            void Buffer(const TYPENAME length, const uint8 buffer[])
            {
                ASSERT(_container != nullptr);

                _offset += _container->SetBuffer<TYPENAME>(_offset, length, buffer);
            }
			void Copy(const uint16 length, const uint8 buffer[])
			{
				ASSERT(_container != nullptr);

				_offset += _container->Copy(_offset, length, buffer);
			}
            template <typename TYPENAME>
            void Number(const TYPENAME value)
            {
                ASSERT(_container != nullptr);

                _offset += _container->SetNumber<TYPENAME>(_offset, value);
            }
            void Boolean(const bool value)
            {
                ASSERT(_container != nullptr);

                _offset += _container->SetBoolean(_offset, value);
            }
            void Text(const string& text)
            {
                ASSERT(_container != nullptr);

                _offset += _container->SetText(_offset, text);
            }
            void NullTerminatedText(const string& text)
            {
                ASSERT(_container != nullptr);

                _offset += _container->SetNullTerminatedText(_offset, text);
            }

        private:
            uint16 _offset;
            FrameType* _container;
        };

    public:
        FrameType()
            : _size(0)
            , _data()
        {
            static_assert(BLOCKSIZE != 0, "This method can only be called if you specify an initial blocksize");
        }
        FrameType(const FrameType<BLOCKSIZE>& copy)
            : _size (copy._size)
            , _data (copy._data) {
        }
        FrameType(uint8 buffer[], const uint32 length, const uint32 loadedSize = 0)
            : _size(loadedSize)
            , _data(buffer, length)
        {
            static_assert(BLOCKSIZE == 0, "This method can only be called if you pass a buffer that can not be extended");
        }
        ~FrameType()
        {
        }

    public:
        inline void Clear()
        {
            _size = 0;
        }
        inline uint32 Size() const
        {
            return (_size);
        }
        inline uint8& operator[](const uint16 index)
        {
            return _data[index];
        }
        inline const uint8& operator[](const uint16 index) const
        {
            return _data[index];
        }
        void Size(uint16 size)
        {
            _data.Allocate(size);

            _size = size;
        }
#ifdef __DEBUG__
        void Dump(const unsigned int offset) const
        {
            static const TCHAR character[] = "0123456789ABCDEF";
            string info;
            uint16 index = offset;

            while (index < _size) {
                if (info.empty() == false) {
                    info += ':';
                }
                info += string("0x") + character[(_data[index] & 0xF0) >> 4] + character[(_data[index] & 0x0F)];
                index++;
            }

            TRACE_L1("MetaData: %s", info.c_str());
        }
#endif

        friend class Reader;
        friend class Writer;

        template <typename TYPENAME>
        uint16 SetBuffer (const uint16 offset, const TYPENAME& length, const uint8 buffer[])
        {
            uint16 requiredLength(sizeof(TYPENAME) + length);

            if ((offset + requiredLength) >= _size) {
                Size(offset + requiredLength);
            }

            SetNumber<TYPENAME>(offset, length);
            ::memcpy(&(_data[offset + sizeof(TYPENAME)]), buffer, length);

            return (requiredLength);
        }
 
		uint16 Copy(const uint16 offset, const uint16 length, uint8 buffer[]) const
		{
			ASSERT(offset + length <= _size);

			::memcpy(buffer, &(_data[offset]), length);

			return (length);
		}
		uint16 Copy(const uint16 offset, const uint16 length, const uint8 buffer[])
		{
			Size(offset + length);

			::memcpy(&(_data[offset]), buffer, length);

			return (length);
		}
        uint16 SetText(const uint16 offset, const string& value)
        {
            std::string convertedText(Core::ToString(value));
            return (SetBuffer<uint16>(offset, static_cast<uint16>(convertedText.length()), reinterpret_cast<const uint8*>(convertedText.c_str())));
        }

        uint16 SetNullTerminatedText(const uint16 offset, const string& value)
        {
            std::string convertedText(Core::ToString(value));
            uint16 requiredLength(convertedText.length() + 1);

            if ((offset + requiredLength) >= _size) {
                Size(offset + requiredLength);
            }

            ::memcpy(&(_data[offset]), convertedText.c_str(), convertedText.length() + 1);

            return (requiredLength);
        }

        template <typename TYPENAME>
        uint16 GetBuffer(const uint16 offset, const TYPENAME length, uint8 buffer[]) const
        {
            TYPENAME textLength;

            ASSERT((offset + sizeof(TYPENAME)) <= _size);

            GetNumber<TYPENAME>(offset, textLength);

            ASSERT((textLength + offset + sizeof(TYPENAME)) <= _size);

            if (textLength + offset + sizeof(TYPENAME) > _size) {
                textLength = (_size - (offset + sizeof(TYPENAME)));
            }

            memcpy (buffer, &(_data[offset + sizeof(TYPENAME)]), (textLength >  length ? length : textLength));

            return (sizeof(TYPENAME) + textLength);
        }

        uint16 GetText(const uint16 offset, string& result) const
        {
            uint16 textLength;

            ASSERT((offset + sizeof(uint16)) <= _size);

            GetNumber<uint16>(offset, textLength);

            ASSERT((textLength + offset + sizeof(uint16)) <= _size);

            if (textLength + offset + sizeof(uint16) > _size) {
                textLength = (_size - (offset + sizeof(uint16)));
            }

            std::string convertedText(reinterpret_cast<const char*>(&(_data[offset + sizeof(uint16)])), textLength);

            result = Core::ToString(convertedText);

            return (sizeof(uint16) + textLength);
        }

        uint16 GetNullTerminatedText(const uint16 offset, string& result) const
        {
            const char* text = reinterpret_cast<const char*>(&(_data[offset]));
            result = text;
            return (result.length() + 1);
        }

        uint16 SetBoolean(const uint16 offset, const bool value)
        {
            if ((offset + 1) >= _size) {
                Size(offset + 1);
            }

            _data[offset] = (value ? 1 : 0);

            return (1);
        }

        uint16 GetBoolean(const uint16 offset, bool& value) const
        {
            ASSERT(offset < _size);

            value = (_data[offset] != 0);

            return (1);
        }

        template <typename TYPENAME>
        inline uint16 SetNumber(const uint16 offset, const TYPENAME number)
        {
            return (SetNumber(offset, number, TemplateIntToType<sizeof(TYPENAME) == 1>()));
        }

        template <typename TYPENAME>
        inline uint16 GetNumber(const uint16 offset, TYPENAME& number) const
        {
            return (GetNumber(offset, number, TemplateIntToType<sizeof(TYPENAME) == 1>()));
        }

    private:
        template <typename TYPENAME>
        uint16 SetNumber(const uint16 offset, const TYPENAME number, const TemplateIntToType<true>&)
        {
            if ((offset + 1) >= _size) {
                Size(offset + 1);
            }

            _data[offset] = static_cast<uint8>(number);

            return (1);
        }

        template <typename TYPENAME>
        uint16 SetNumber(const uint16 offset, const TYPENAME number, const TemplateIntToType<false>&)
        {
            if ((offset + sizeof(TYPENAME)) >= _size) {
                Size(offset + sizeof(TYPENAME));
            }

#ifdef LITTLE_ENDIAN_PLATFORM
            {
                const uint8* source = reinterpret_cast<const uint8*>(&number);
                uint8* destination = &(_data[offset + sizeof(TYPENAME) - 1]);

                *destination-- = *source++;
                *destination-- = *source++;

                if ((sizeof(TYPENAME) == 4) || (sizeof(TYPENAME) == 8)) {
                    *destination-- = *source++;
                    *destination-- = *source++;

                    if (sizeof(TYPENAME) == 8) {
                        *destination-- = *source++;
                        *destination-- = *source++;
                        *destination-- = *source++;
                        *destination-- = *source++;
                    }
                }
            }
#else
            {
                const uint8* source = reinterpret_cast<const uint8*>(&number);
                uint8* destination = &(_data[offset]);

                *destination++ = *source++;
                *destination++ = *source++;

                if ((sizeof(TYPENAME) == 4) || (sizeof(TYPENAME) == 8)) {
                    *destination++ = *source++;
                    *destination++ = *source++;

                    if (sizeof(TYPENAME) == 8) {
                        *destination++ = *source++;
                        *destination++ = *source++;
                        *destination++ = *source++;
                        *destination++ = *source++;
                    }
                }
            }
#endif

            return (sizeof(TYPENAME));
        }
 
        template <typename TYPENAME>
        uint16 GetNumber(const uint16 offset, TYPENAME& number, const TemplateIntToType<true>&) const
        {
            // Only on package level allowed to pass the boundaries!!!
            ASSERT((offset + sizeof(TYPENAME)) <= _size);

            number = static_cast<TYPENAME>(_data[offset]);

            return (1);
        }
 
        template <typename TYPENAME>
        inline uint16 GetNumber(const uint16 offset, TYPENAME& value, const TemplateIntToType<false>&) const
        {
            // Only on package level allowed to pass the boundaries!!!
            ASSERT((offset + sizeof(TYPENAME)) <= _size);

            TYPENAME result;

#ifdef LITTLE_ENDIAN_PLATFORM
            {
                const uint8* source = &(_data[offset]);
                uint8* destination = &(reinterpret_cast<uint8*>(&result)[sizeof(TYPENAME) - 1]);

                *destination-- = *source++;
                *destination-- = *source++;

                if ((sizeof(TYPENAME) == 4) || (sizeof(TYPENAME) == 8)) {
                    *destination-- = *source++;
                    *destination-- = *source++;

                    if (sizeof(TYPENAME) == 8) {
                        *destination-- = *source++;
                        *destination-- = *source++;
                        *destination-- = *source++;
                        *destination-- = *source++;
                    }
                }
            }
#else
            {
                // If the sizeof > 1, the alignment could be wrong. Assume the worst, always copy !!!
                const uint8* source = &(_data[offset]);
                uint8* destination = reinterpret_cast<uint8*>(&result);

                *destination++ = *source++;
                *destination++ = *source++;

                if ((sizeof(TYPENAME) == 4) || (sizeof(TYPENAME) == 8)) {
                    *destination++ = *source++;
                    *destination++ = *source++;

                    if (sizeof(TYPENAME) == 8) {
                        *destination++ = *source++;
                        *destination++ = *source++;
                        *destination++ = *source++;
                        *destination++ = *source++;
                    }
                }
            }
#endif

            value = result;

            return (sizeof(TYPENAME));
        }

	private:
        mutable uint16 _size;
        AllocatorType<BLOCKSIZE> _data;
    };
}
}

#endif //__GENERICS_FRAME_H
