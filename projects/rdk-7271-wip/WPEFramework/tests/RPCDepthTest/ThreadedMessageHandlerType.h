#pragma once

#include "../../generics/generics.h"
#include "../../rpc/rpc.h"
#include "../../tracing/tracing.h"

namespace WPEFramework
{
    namespace RPC
    {
        template <typename MESSAGETYPE, const uint32 MESSAGESLOTS = 4, const uint16 THREADPOOLCOUNT = 2>
        class ThreadedMessageHandlerType : public Core::IPCServerType<MESSAGETYPE> {
        private:
            class Info {
            public:
                Info()
                    : _message()
                    , _channel()
                {
                }
                Info(Core::ProxyType<MESSAGETYPE> message, Core::ProxyType<Core::IPCChannel> channel)
                    : _message(message)
                    , _channel(channel)
                {
                }
                Info(const Info& copy)
                    : _message(copy._message)
                    , _channel(copy._channel)
                {
                }
                ~Info()
                {
                }
                Info& operator=(const Info& rhs)
                {
                    _message = rhs._message;
                    _channel = rhs._channel;

                    return (*this);
                }

            public:
                inline void Dispatch()
                {

//                    Administrator::Instance().Invoke(_channel, _message);
//                    Core::ProxyType<Core::IIPC> response(Core::proxy_cast<Core::IIPC>(_message));
//                    _channel->ReportResponse(response);

                    // TODO: relay message back to handler in server/client
                    std::cerr << "Dispatch: " << _message->Parameters().Value() << std::endl;

                    //_message->
                    //Core::ProxyType<Core::IIPC> response(Core::proxy_cast<Core::IIPC>(_message));

                }

            private:
                Core::ProxyType<MESSAGETYPE> _message;
                Core::ProxyType<Core::IPCChannel> _channel;
            };

            ThreadedMessageHandlerType(const ThreadedMessageHandlerType<MESSAGETYPE, THREADPOOLCOUNT, MESSAGESLOTS>&);
            ThreadedMessageHandlerType<MESSAGETYPE, THREADPOOLCOUNT, MESSAGESLOTS>& operator=(const ThreadedMessageHandlerType<MESSAGETYPE, THREADPOOLCOUNT, MESSAGESLOTS>&);

        public:
            ThreadedMessageHandlerType()
                : _threadPoolEngine(0, _T("IPCInterfaceMessageHandler"))
            {
            }
            virtual ~ThreadedMessageHandlerType()
            {
            }

        public:
            virtual void Procedure(Core::IPCChannel& channel, Core::ProxyType<MESSAGETYPE>& data)
            {
                // Oke, see if we can reference count the IPCChannel
                Core::ProxyType<Core::IPCChannel> refChannel(Core::ProxyType<Core::IPCChannel>(dynamic_cast<Core::IProxy*>(&channel)));

                ASSERT(refChannel.IsValid());

                if (refChannel.IsValid() == true) {
                    if (_threadPoolEngine.Pending() >= ((MESSAGESLOTS * 80) / 100)) {
                        TRACE_L1("_threadPoolEngine.Pending() == %d", _threadPoolEngine.Pending());
                    }
                    _threadPoolEngine.Submit(Info(data, refChannel), Core::infinite);
                }
            }

        private:
            Core::ThreadPoolType<Info, THREADPOOLCOUNT, MESSAGESLOTS> _threadPoolEngine;
        };
    }
}
