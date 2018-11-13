#include "interfaces.h"

#include <WPEFramework/com/IUnknown.h> // MethodHandler, UnknownProxyType
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
    };

    typedef ProxyStub::StubType<Exchange::IAdder, AdderStubMethods, ProxyStub::UnknownStub> AdderStub;

    class AdderProxy : public ProxyStub::UnknownProxyType<Exchange::IAdder> {
    public:
        AdderProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation,
            const bool otherSideInformed)
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

    };

    namespace {
        class Instantiation {
        public:
            Instantiation()
            {
                RPC::Administrator::Instance().Announce<Exchange::IAdder, AdderProxy, AdderStub>();
            }

            ~Instantiation()
            {
            }

        } instantiation;
    }
}
