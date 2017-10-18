#ifndef __RPC_MESSAGES_H
#define __RPC_MESSAGES_H

#include "Module.h"

namespace WPEFramework {
namespace RPC {

    namespace Data {
        static const uint16 IPC_BLOCK_SIZE = 512;

        class Frame : public Core::FrameType<IPC_BLOCK_SIZE> {
        private:
            Frame(Frame&);
            Frame& operator=(const Frame&);

        public:
            Frame()
            {
            }
            ~Frame()
            {
            }

        public:
            friend class Input;
            friend class Output;
            friend class ObjectInterface;

            uint16 Serialize(const uint16 offset, uint8 stream[], const uint16 maxLength) const
            {
                uint16 copiedBytes((Size() - offset) > maxLength ? maxLength : (Size() - offset));

                ::memcpy(stream, &(operator[](offset)), copiedBytes);

                return (copiedBytes);
            }
            uint16 Deserialize(const uint16 offset, const uint8 stream[], const uint16 maxLength)
            {
                Size(offset + maxLength);

                ::memcpy(&(operator[](offset)), stream, maxLength);

                return (maxLength);
            }
        };

        class Input {
        private:
            Input(const Input&);
            Input& operator=(const Input&);

        public:
            Input()
                : _data()
            {
            }
            ~Input()
            {
            }

        public:
            inline void Clear()
            {
                _data.Clear();
            }
            void Set(void* implementation, const uint32 interfaceId, const uint8 methodId)
            {
                uint16 result = _data.SetNumber<void*>(0, implementation);
                result += _data.SetNumber<uint32>(result, interfaceId);
                _data.SetNumber(result, methodId);
            }
            template <typename TYPENAME>
            TYPENAME* Implementation()
            {
                void* result = nullptr;

                _data.GetNumber<void*>(0, result);

                return (static_cast<TYPENAME*>(result));
            }
            uint32 InterfaceId() const
            {
                uint32 result = 0;

                _data.GetNumber<uint32>(sizeof(void*), result);

                return (result);
            }
            uint8 MethodId() const
            {
                uint8 result = 0;

                _data.GetNumber(sizeof(void*) + sizeof(uint32), result);

                return (result);
            }
            uint32 Length() const
            {
                return (_data.Size());
            }
            inline Frame::Writer Writer()
            {
                return (Frame::Writer(_data, (sizeof(void*) + sizeof(uint32) + sizeof(uint8))));
            }
            inline const Frame::Reader Reader() const
            {
                return (Frame::Reader(_data, (sizeof(void*) + sizeof(uint32) + sizeof(uint8))));
            }
            uint16 Serialize(uint8 stream[], const uint16 maxLength, const uint32 offset) const
            {
                return (_data.Serialize(static_cast<uint16>(offset), stream, maxLength));
            }
            uint16 Deserialize(const uint8 stream[], const uint16 maxLength, const uint32 offset)
            {
                return (_data.Deserialize(static_cast<uint16>(offset), stream, maxLength));
            }

        private:
            Frame _data;
        };

        class Output {
        private:
            Output(const Output&);
            Output& operator=(const Output&);

        public:
            Output()
                : _data()
            {
            }
            ~Output()
            {
            }

        public:
            inline void Clear()
            {
                _data.Clear();
            }
            inline Frame::Writer Writer()
            {
                return (Frame::Writer(_data, 0));
            }
            inline const Frame::Reader Reader() const
            {
                return (Frame::Reader(_data, 0));
            }
            inline uint32 Length() const
            {
                return (static_cast<uint32>(_data.Size()));
            }
            inline uint16 Serialize(uint8 stream[], const uint16 maxLength, const uint32 offset) const
            {
                return (_data.Serialize(static_cast<uint16>(offset), stream, maxLength));
            }
            inline uint16 Deserialize(const uint8 stream[], const uint16 maxLength, const uint32 offset)
            {
                return (_data.Deserialize(static_cast<uint16>(offset), stream, maxLength));
            }

        private:
            Frame _data;
        };

        class Init {
        private:
            Init(const Init&);
            Init& operator=(const Init&);

        public:
            Init()
                : _implementation(nullptr)
                , _interfaceId(~0)
                , _exchangeId(~0)
            {
            }
            Init(Core::IUnknown* implementation, const uint32 interfaceId, const uint32 exchangeId)
                : _implementation(implementation)
                , _interfaceId(interfaceId)
                , _exchangeId(exchangeId)
            {
            }
            ~Init()
            {
            }

        public:
            void Set(const uint32 exchangeId, const uint32 interfaceId, void* implementation)
            {
                _implementation = implementation;
                _interfaceId = interfaceId;
                _exchangeId = exchangeId;
            }
            void* Implementation()
            {
                return (_implementation);
            }
            uint32 InterfaceId() const
            {
                return (_interfaceId);
            }
            uint32 ExchangeId() const
            {
                return (_exchangeId);
            }

        private:
            void* _implementation;
            uint32 _interfaceId;
            uint32 _exchangeId;
        };

        class ObjectInterface {
        private:
            ObjectInterface(const ObjectInterface&);
            ObjectInterface& operator=(const ObjectInterface&);

        public:
            ObjectInterface()
                : _data()
            {
            }
            ~ObjectInterface()
            {
            }

        public:
            inline void Clear()
            {
                _data.Clear();
            }
            void Set(const string& className, const uint32 versionId, const uint32 interfaceId)
            {
                _data.SetNumber<uint32>(0, versionId);
                _data.SetNumber<uint32>(4, interfaceId);
                _data.SetText(8, className);
            }
            string ClassName() const
            {
                string className;

                _data.GetText(8, className);

                return (className);
            }
            uint32 InterfaceId() const
            {
                uint32 interfaceId;

                _data.GetNumber<uint32>(4, interfaceId);

                return (interfaceId);
            }
            uint32 VersionId() const
            {
                uint32 versionId;

                _data.GetNumber<uint32>(0, versionId);

                return (versionId);
            }
            uint32 Length() const
            {
                return (_data.Size());
            }
            inline Frame::Writer Writer()
            {
                return (Frame::Writer(_data, (sizeof(void*) + sizeof(uint32) + sizeof(uint8))));
            }
            inline const Frame::Reader Reader() const
            {
                return (Frame::Reader(_data, (sizeof(void*) + sizeof(uint32) + sizeof(uint8))));
            }
            uint16 Serialize(uint8 stream[], const uint16 maxLength, const uint32 offset) const
            {
                return (_data.Serialize(static_cast<uint16>(offset), stream, maxLength));
            }
            uint16 Deserialize(const uint8 stream[], const uint16 maxLength, const uint32 offset)
            {
                return (_data.Deserialize(static_cast<uint16>(offset), stream, maxLength));
            }

        private:
            Frame _data;
        };
    }

    typedef Core::IPCMessageType<1, Data::Init, Core::IPC::ScalarType<string> > AnnounceMessage;
    typedef Core::IPCMessageType<2, Data::Input, Data::Output> InvokeMessage;
    typedef Core::IPCMessageType<3, Data::ObjectInterface, Core::IPC::ScalarType<void*> > ObjectMessage;
}
}

#endif // __RPC_MESSAGES_H
