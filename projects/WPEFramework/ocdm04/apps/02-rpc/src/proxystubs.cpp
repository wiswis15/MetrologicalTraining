#include "interfaces.h"

#include <WPEFramework/com/IUnknown.h>
#include <WPEFramework/com/com.h>

namespace WPEFramework {
    ProxyStub::MethodHandler AdderStubMethods[] = {
          [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
              //
              // virtual uint32_t GetValue = 0;
              //
              RPC::Data::Frame::Writer response(message->Response().Writer());

              response.Number(message->Parameters().Implementation<Exchange::IAdder>()->GetValue());
          },
          [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
              //
              // virtual void Add(uint32_t value) = 0;
              //
              RPC::Data::Frame::Reader parameters(message->Parameters().Reader());

              uint32_t value = parameters.Number<uint32_t>();

              message->Parameters().Implementation<Exchange::IAdder>()->Add(value);
          },
          [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
              //
              // virtual pid_t GetPid() = 0;
              //
              RPC::Data::Frame::Writer response(message->Response().Writer());

              response.Number(message->Parameters().Implementation<Exchange::IAdder>()->GetPid());
          },
    };

    class AdderProxy final : public ProxyStub::UnknownProxyType<Exchange::IAdder> {
    public:
        AdderProxy(const Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }

        virtual ~AdderProxy()
        {
        }

        virtual uint32_t GetValue()
        {
            IPCMessage newMessage(BaseClass::Message(0));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<uint32_t>());
        }

        virtual void Add(uint32_t value)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number(value);

            Invoke(newMessage);
        }

        virtual pid_t GetPid()
        {
            IPCMessage newMessage(BaseClass::Message(2));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<pid_t>());
        }
    };

    namespace {
        typedef ProxyStub::UnknownStubType<Exchange::IAdder, AdderStubMethods> AdderStub;

        static class Instantiation {
        public:
            Instantiation()
            {
                RPC::Administrator::Instance().Announce<Exchange::IAdder, AdderProxy, AdderStub>();
            }
        } instantiation;
    }
}
