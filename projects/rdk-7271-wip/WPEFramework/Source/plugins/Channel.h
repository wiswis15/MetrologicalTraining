#ifndef __PLUGIN_FRAMEWORK_CHANNEL__
#define __PLUGIN_FRAMEWORK_CHANNEL__

#include "Module.h"
#include "Request.h"

namespace WPEFramework {
namespace PluginHost {

    class EXTERNAL Channel : public Web::WebSocketLinkType<Core::SocketStream, Request, Web::Response, RequestPool&> {
    private:
        typedef Web::WebSocketLinkType<Core::SocketStream, Request, Web::Response, RequestPool&> BaseClass;

        class EXTERNAL SerializerImpl : public Core::JSON::IElement::Serializer {
        private:
            SerializerImpl() = delete;
            SerializerImpl(const SerializerImpl&) = delete;
            SerializerImpl& operator=(const SerializerImpl&) = delete;

        public:
            SerializerImpl(Channel& parent)
                : _parent(parent)
                , _sendQueue(5)
            {
            }
            ~SerializerImpl()
            {
            }

        public:
            inline bool IsIdle() const
            {
                return (_sendQueue.Count() == 0);
            }
            inline void Submit(const Core::ProxyType<Core::JSON::IElement>& entry)
            {
                _parent.Lock();

                if (_parent.IsOpen() == true) {
                    _sendQueue.Add(const_cast<Core::ProxyType<Core::JSON::IElement>&>(entry));

                    if (_sendQueue.Count() == 1) {
                        Core::JSON::IElement::Serializer::Submit(*entry);

                        _parent.Trigger();
                    }
                }
                _parent.Unlock();
            }

        private:
            virtual void Serialized(const Core::JSON::IElement& element)
            {
                _parent.Lock();

                Core::ProxyType<Core::JSON::IElement> current;

                _sendQueue.Remove(0, current);

                ASSERT(&(*(current)) == &element);
                DEBUG_VARIABLE(element);

                _parent.Send(current);

                if (_sendQueue.Count() > 0) {
                    Core::JSON::IElement::Serializer::Submit(*(_sendQueue[0]));
                }

                _parent.Unlock();
            }

        private:
            Channel& _parent;
            Core::ProxyList<Core::JSON::IElement> _sendQueue;
        };
        class EXTERNAL DeserializerImpl : public Core::JSON::IElement::Deserializer {
        private:
            DeserializerImpl() = delete;
            DeserializerImpl(const DeserializerImpl&) = delete;
            DeserializerImpl& operator=(const DeserializerImpl&) = delete;

        public:
            DeserializerImpl(Channel& parent)
                : _parent(parent)
            {
            }
            ~DeserializerImpl()
            {
            }

        public:
            inline bool IsIdle() const
            {
                return (_current.IsValid() == false);
            }

        private:
            virtual Core::JSON::IElement* Element(const string& identifier)
            {
                if (_parent.IsOpen() == true) {
                    _current = _parent.Element(identifier);

                    return (_current.IsValid() == true ? &(*_current) : NULL);
                }
                return (NULL);
            }
            virtual void Deserialized(Core::JSON::IElement& element)
            {
                ASSERT(&element == &(*_current));
                DEBUG_VARIABLE(element);

                _parent.Received(_current);

                _current.Release();
            }

        private:
            Channel& _parent;
            Core::ProxyType<Core::JSON::IElement> _current;
        };

        Channel() = delete;
        Channel(const Channel& copy) = delete;
        Channel& operator=(const Channel&) = delete;

    public:
        enum ChannelState {
            CLOSED = 0x01,
            WEB = 0x02,
            JSON = 0x04,
            RAW = 0x08
        };

    public:
        Channel(const SOCKET& connector, const Core::NodeId& remoteId);
        virtual ~Channel();

    public:
        inline bool HasActivity() const
        {
            bool forcedActivity((_state & 0x4000) == 0x4000);
            bool result(BaseClass::HasActivity());

            if ((forcedActivity == true) || ((result == false) && (IsWebSocket() == true))) {
                Lock();

                if (forcedActivity == true) {
                    _state &= (~0x4000);
                }
                else {
                    // We can try to fire a ping/pong
                    _state |= 0x4000;
                    const_cast<BaseClass*>(static_cast<const BaseClass*>(this))->Ping();
                    result = true;
                }

                Unlock();
            }

            return (result);
        }
        const string Name() const
        {
            return string(_nameOffset != static_cast<uint32>(~0) ? &(BaseClass::Path().c_str()[_nameOffset]) : EMPTY_STRING);
        }
        inline uint32 Id() const
        {
            return (_ID);
        }
        inline ChannelState State() const
        {
            return static_cast<ChannelState>(_state & 0x0FFF);
        }
        inline bool IsNotified() const
        {
            return ((_state & 0x8000) != 0);
        }
        inline void Submit(const string& notification)
        {
            Lock();

            if (IsOpen() == true) {
                _sendQueue.push_back(notification);

                if (_sendQueue.size() == 1) {
                    _offset = 0;
                    Trigger();
                }
            }
            Unlock();
        }
        inline void Submit(const Core::ProxyType<Core::JSON::IElement>& entry)
        {
            _serializer.Submit(entry);
        }
        inline void Submit(const Core::ProxyType<Web::Response>& entry)
        {
            BaseClass::Submit(entry);
        }
        inline void RequestOutbound()
        {
            BaseClass::Trigger();
        }

    protected:
        inline void SetId(const uint32 id)
        {
            _ID = id;
        }
        inline void Lock() const
        {
            _adminLock.Lock();
        }
        inline void Unlock() const
        {
            _adminLock.Unlock();
        }
        inline void Properties(const uint32 offset)
        {
            _nameOffset = offset;
        }
        inline void State(const ChannelState state, const bool notification)
        {
            Binary(state == RAW);
            _state = state | (notification ? 0x8000 : 0x0000);
        }
        inline uint16 Serialize(uint8* dataFrame, const uint16 maxSendSize)
        {
            uint16 size = 0;

            if (_sendQueue.size() != 0) {
                while ((_sendQueue.size() != 0) && (size < maxSendSize)) {
                    const string& data(_sendQueue.front());
                    uint16 neededBytes(static_cast<uint16>(data.length()) - _offset);

                    if ((size + neededBytes) <= maxSendSize) {
                        // Seems we need to send plain strings...
                        ::memcpy(dataFrame, &(data.c_str()[_offset]), neededBytes);
                        size += neededBytes;
                        _offset = 0;

                        // See if there is more to do..
                        _sendQueue.pop_front();
                    }
                    else {
                        // Seems we need to send plain strings...
                        uint16 addedBytes = maxSendSize - size;
                        ::memcpy(dataFrame, &(data.c_str()[_offset]), addedBytes);
                        _offset += addedBytes;
                        size += addedBytes;
                    }
                }
            }
            else {
                size = _serializer.Serialize(dataFrame, maxSendSize);
            }

            return (size);
        }
        inline uint16 Deserialize(const uint8* dataFrame, const uint16 receivedSize)
        {
            return (_deserializer.Deserialize(dataFrame, receivedSize));
        }

    private:
        // Handle the WebRequest coming in.
        virtual void LinkBody(Core::ProxyType<Request>& request) = 0;
        virtual void Received(Core::ProxyType<Request>& request) = 0;
        virtual void Send(const Core::ProxyType<Web::Response>& response) = 0;

        // Handle the JSON structs flowing over the WebSocket.
        // [INBOUND]  Completed deserialized JSON objects that are Received, will trigger the Received.
        // [OUTBOUND] Completed serialized JSON objects that are send out, will trigger the Send.
        virtual Core::ProxyType<Core::JSON::IElement> Element(const string& identifier) = 0;
        virtual void Send(const Core::ProxyType<Core::JSON::IElement>& element) = 0;
        virtual void Received(Core::ProxyType<Core::JSON::IElement>& element) = 0;

        // We are in an upgraded mode, we are a websocket. Time to "deserialize and serialize
        // INBOUND and OUTBOUND information.
        virtual uint16 SendData(uint8* dataFrame, const uint16 maxSendSize) = 0;
        virtual uint16 ReceiveData(uint8* dataFrame, const uint16 receivedSize) = 0;

        // Whenever there is a state change on the link, it is reported here.
        virtual void StateChange() = 0;

        virtual bool IsIdle() const
        {
            return ((BaseClass::IsWebSocket() == false) || ((_serializer.IsIdle() == true) && (_deserializer.IsIdle() == true)));
        }

    private:
        mutable Core::CriticalSection _adminLock;
        uint32 _ID;
        uint32 _nameOffset;
        mutable uint32 _state;
        SerializerImpl _serializer;
        DeserializerImpl _deserializer;
        uint32 _offset;
        std::list<string> _sendQueue;

        // All requests needed by any instance of this webserver are coming from this web server. They are extracted
        // from a ppol. If the request is nolonger needed, the resuest returns to the pool.
        static RequestPool _requestAllocator;
    };
}
} // namespace Server

#endif // __PLUGIN_FRAMEWORK_CHANNEL__
