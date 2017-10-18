
#include "IBrowser.h"
#include "INetflix.h"
#include "IDictionary.h"
#include "IWebServer.h"
#include "IProvisioning.h"
#include "IWebDriver.h"
#include "IOpenCDMi.h"
#include "IResourceCenter.h"
#include "IComposition.h"

MODULE_NAME_DECLARATION(BUILDREF_WEBBRIDGE)

namespace WPEFramework {

namespace ProxyStubs {

    using namespace Exchange;

    // -------------------------------------------------------------------------------------------
    // STUB
    // -------------------------------------------------------------------------------------------

    ProxyStub::MethodHandler BrowserStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

            // virtual void SetURL(const string& URL) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());

            message->Parameters().Implementation<IBrowser>()->SetURL(parameters.Text());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

            // virtual string GetURL() const = 0;
            RPC::Data::Frame::Writer response(message->Response().Writer());
            response.Text(message->Parameters().Implementation<IBrowser>()->GetURL());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Writer response(message->Response().Writer());
            response.Number(message->Parameters().Implementation<IBrowser>()->GetFPS());
        },
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            IBrowser::INotification* implementation = reader.Number<IBrowser::INotification*>();
            IBrowser::INotification* proxy = RPC::Administrator::Instance().CreateProxy<IBrowser::INotification>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for IBrowserNotification: %p"), implementation);
            }
            else {
                parameters.Implementation<IBrowser>()->Register(proxy);
                if (proxy->Release() != Core::ERROR_NONE) {
                    TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
                }
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            // Need to find the proxy that goes with the given implementation..
            IBrowser::INotification* stub = reader.Number<IBrowser::INotification*>();

            // NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
            IBrowser::INotification* proxy = RPC::Administrator::Instance().FindProxy<IBrowser::INotification>(stub);

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not find stub for IBrowserNotification: %p"), stub);
            }
            else {
                parameters.Implementation<IBrowser>()->Unregister(proxy);
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

            // virtual void SetURL(const string& URL) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());

            message->Parameters().Implementation<IBrowser>()->Hide(parameters.Boolean());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            message->Parameters().Implementation<IBrowser>()->Go();
        },
        nullptr
    };

    ProxyStub::MethodHandler BrowserNotificationStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void LoadFinished (const string& URL) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string URL(parameters.Text());

            message->Parameters().Implementation<IBrowser::INotification>()->LoadFinished(URL);
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void URLChanged (const string& URL) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string URL(parameters.Text());

            ASSERT(message.IsValid() == true);
            ASSERT(message->Parameters().Implementation<IBrowser::INotification>() != nullptr);

            message->Parameters().Implementation<IBrowser::INotification>()->URLChanged(URL);
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void URLChanged (const string& URL) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            bool hidden(parameters.Boolean());

            ASSERT(message.IsValid() == true);
            ASSERT(message->Parameters().Implementation<IBrowser::INotification>() != nullptr);

            message->Parameters().Implementation<IBrowser::INotification>()->Hidden(hidden);
        },

        nullptr
    };

    ProxyStub::MethodHandler WebDriverStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {

            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer writer(parameters.Writer());

            PluginHost::IShell* implementation = reader.Number<PluginHost::IShell*>();
            PluginHost::IShell* proxy = RPC::Administrator::Instance().CreateProxy<PluginHost::IShell>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for WebDriver: %p"), implementation);
                writer.Number<uint32>(Core::ERROR_RPC_CALL_FAILED);
            }
            else {
                writer.Number(parameters.Implementation<IWebDriver>()->Configure(proxy));
                if (proxy->Release() != Core::ERROR_NONE) {
                    TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
                }
            }
        },
        nullptr
    };

    ProxyStub::MethodHandler OpenCDMiStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {

            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer writer(parameters.Writer());

            PluginHost::IShell* implementation = reader.Number<PluginHost::IShell*>();
            PluginHost::IShell* proxy = RPC::Administrator::Instance().CreateProxy<PluginHost::IShell>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for OpenCDMi: %p"), implementation);
                writer.Number<uint32>(Core::ERROR_RPC_CALL_FAILED);
            }
            else {
                writer.Number(parameters.Implementation<IOpenCDMi>()->Configure(proxy));
                if (proxy->Release() != Core::ERROR_NONE) {
                    TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
                }
            }
        },
        nullptr
    };

    ProxyStub::MethodHandler NetflixStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            INetflix::INotification* implementation = reader.Number<INetflix::INotification*>();
            INetflix::INotification* proxy = RPC::Administrator::Instance().CreateProxy<INetflix::INotification>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for INetflixNotification: %p"), implementation);
            }
            else {
                parameters.Implementation<INetflix>()->Register(proxy);
                if (proxy->Release() != Core::ERROR_NONE) {
                    TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
                }
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            // Need to find the proxy that goes with the given implementation..
            INetflix::INotification* stub = reader.Number<INetflix::INotification*>();

            // NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
            INetflix::INotification* proxy = RPC::Administrator::Instance().FindProxy<INetflix::INotification>(stub);

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not find stub for IBrowserNotification: %p"), stub);
            }
            else {
                parameters.Implementation<INetflix>()->Unregister(proxy);
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Writer response(message->Response().Writer());
            response.Text(message->Parameters().Implementation<INetflix>()->GetESN());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            message->Parameters().Implementation<INetflix>()->Go();
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string element(parameters.Text());
            message->Parameters().Implementation<INetflix>()->SystemCommand(element);
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string element(parameters.Text());
            message->Parameters().Implementation<INetflix>()->Language(element);
        },
        nullptr
    };

    ProxyStub::MethodHandler NetflixNotificationStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

            // virtual void StateChange (const uint32 newState) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            INetflix::state newState(parameters.Number<INetflix::state>());

            message->Parameters().Implementation<INetflix::INotification>()->StateChange(newState);
        },
        nullptr
    };

    ProxyStub::MethodHandler ResourceCenterStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {

            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer writer(message->Response().Writer());

            writer.Number(parameters.Implementation<IResourceCenter>()->Configure(reader.Text()));
        },
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            IResourceCenter::INotification* implementation = reader.Number<IResourceCenter::INotification*>();
            IResourceCenter::INotification* proxy = RPC::Administrator::Instance().CreateProxy<IResourceCenter::INotification>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for IResourceCenter::INotification: %p"), implementation);
            }
            else {
                parameters.Implementation<IResourceCenter>()->Register(proxy);
                if (proxy->Release() == Core::ERROR_DESTRUCTION_FAILED) {
                    TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
                }
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            // Need to find the proxy that goes with the given implementation..
            IResourceCenter::INotification* stub = reader.Number<IResourceCenter::INotification*>();

            // NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
            IResourceCenter::INotification* proxy = RPC::Administrator::Instance().FindProxy<IResourceCenter::INotification>(stub);

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not find stub for IResourceCenter::INotification: %p"), stub);
            }
            else {
                parameters.Implementation<IResourceCenter>()->Unregister(proxy);
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Writer response(message->Response().Writer());
            response.Number<Exchange::IResourceCenter::hardware_state>(message->Parameters().Implementation<IResourceCenter>()->HardwareState());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Frame::Writer response(message->Response().Writer());
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            uint8 length = parameters.Number<uint8>();
            uint8 buffer[256];

            length = message->Parameters().Implementation<IResourceCenter>()->Identifier(length, buffer);
            response.Buffer(length, buffer);
        },
        nullptr
    };

    ProxyStub::MethodHandler ResourceCenterNotificationStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            // virtual void StateChange (Exchange::IResourceCenter::hardware_state state) = 0;
            message->Parameters().Implementation<IResourceCenter::INotification>()->StateChange(reader.Number<Exchange::IResourceCenter::hardware_state>());
        },
        nullptr
    };

    ProxyStub::MethodHandler ProvisioningStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            IProvisioning::INotification* implementation = reader.Number<IProvisioning::INotification*>();
            IProvisioning::INotification* proxy = RPC::Administrator::Instance().CreateProxy<IProvisioning::INotification>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for IProvisioningNotification: %p"), implementation);
            }
            else {
                parameters.Implementation<IProvisioning>()->Register(proxy);
                proxy->Release();
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            // Need to find the proxy that goes with the given implementation..
            IProvisioning::INotification* stub = reader.Number<IProvisioning::INotification*>();

            // NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
            IProvisioning::INotification* proxy = RPC::Administrator::Instance().FindProxy<IProvisioning::INotification>(stub);

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not find a stub for IProvisioningNotification: %p"), stub);
            }
            else {
                parameters.Implementation<IProvisioning>()->Unregister(proxy);
            }
        },
        nullptr
    };

    ProxyStub::MethodHandler ProvisioningNotificationStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Provisioned(const string& ) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string element(parameters.Text());

            message->Parameters().Implementation<IProvisioning::INotification>()->Provisioned(element);
        },
        nullptr
    };

    ProxyStub::MethodHandler CompositionStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            IComposition::INotification* implementation = reader.Number<IComposition::INotification*>();
            IComposition::INotification* proxy = RPC::Administrator::Instance().CreateProxy<IComposition::INotification>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for ICompositionNotification: %p"), implementation);
            }
            else {
                parameters.Implementation<IComposition>()->Register(proxy);
                proxy->Release();
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            // Need to find the proxy that goes with the given implementation..
            IComposition::INotification* stub = reader.Number<IComposition::INotification*>();

            // NOTE: FindProxy does *NOT* AddRef the result. Do not release what is obtained via FindProxy..
            IComposition::INotification* proxy = RPC::Administrator::Instance().FindProxy<IComposition::INotification>(stub);

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not find a stub for ICompositionNotification: %p"), stub);
            }
            else {
                parameters.Implementation<IComposition>()->Unregister(proxy);
            }
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer output(message->Response().Writer());

            output.Number<IComposition::IClient*>(parameters.Implementation<IComposition>()->Client(reader.Number<uint8>()));
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer output(message->Response().Writer());

            output.Number<IComposition::IClient*>(parameters.Implementation<IComposition>()->Client(reader.Text()));
        },

        nullptr
    };

    ProxyStub::MethodHandler CompositionClientStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual string Name() const = 0;
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Writer writer(message->Response().Writer());

            writer.Text(parameters.Implementation<IComposition::IClient>()->Name());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Kill() = 0;
            RPC::Data::Input& parameters(message->Parameters());

            parameters.Implementation<IComposition::IClient>()->Kill();
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Focus() = 0;
            RPC::Data::Input& parameters(message->Parameters());

            parameters.Implementation<IComposition::IClient>()->Focus();
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Opacity(const uint32 value) = 0;
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            parameters.Implementation<IComposition::IClient>()->Opacity(reader.Number<uint32>());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Geometry(const uint32 X, const uint32 Y, const uint32 width, const uint32 height) = 0;
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            uint32 X(reader.Number<uint32>());
            uint32 Y(reader.Number<uint32>());
            uint32 width(reader.Number<uint32>());
            uint32 height(reader.Number<uint32>());

            parameters.Implementation<IComposition::IClient>()->Geometry(X, Y, width, height);
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void Visible(const bool visible) = 0;
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());

            parameters.Implementation<IComposition::IClient>()->Visible(reader.Boolean());
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void SetTop() = 0;
            RPC::Data::Input& parameters(message->Parameters());

            parameters.Implementation<IComposition::IClient>()->SetTop();
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void SetInput() = 0;
            RPC::Data::Input& parameters(message->Parameters());

            parameters.Implementation<IComposition::IClient>()->SetInput();
        },
        nullptr
    };

    ProxyStub::MethodHandler CompositionNotificationStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer writer(message->Response().Writer());

            IComposition::IClient* implementation = reader.Number<IComposition::IClient*>();
            IComposition::IClient* proxy = RPC::Administrator::Instance().CreateProxy<IComposition::IClient>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for IComposition::IClient %p"), implementation);
            }
            else {
                parameters.Implementation<IComposition::INotification>()->Attached(proxy);
                if (proxy->Release() == Core::ERROR_DESTRUCTION_FAILED) {
                    TRACE_L1("Oops seems like we could not release this sink properly. %d", __LINE__);
                }
            }
        },
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& parameters(message->Parameters());
            RPC::Data::Frame::Reader reader(parameters.Reader());
            RPC::Data::Frame::Writer writer(message->Response().Writer());

            IComposition::IClient* implementation = reader.Number<IComposition::IClient*>();
            IComposition::IClient* proxy = RPC::Administrator::Instance().CreateProxy<IComposition::IClient>(channel, implementation, true, false);

            ASSERT((proxy != nullptr) && "Failed to create proxy");

            if (proxy == nullptr) {
                TRACE_L1(_T("Could not create a stub for IComposition::IClient %p"), implementation);
            }
            else {
                parameters.Implementation<IComposition::INotification>()->Detached(proxy);
                if (proxy->Release() == Core::ERROR_DESTRUCTION_FAILED) {
                    TRACE_L1("Oops seems like we could not release this sink properly. %d", __LINE__);
                }
            }
        },
        nullptr
    };

    ProxyStub::MethodHandler WebServerStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void AddProxy (const string& path, const string& subst, const string& address) = 0;

            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string path(parameters.Text());
            string subst(parameters.Text());
            string address(parameters.Text());

            message->Parameters().Implementation<IWebServer>()->AddProxy(path, subst, address);
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual void RemoveProxy (const string& path) = 0;
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string path(parameters.Text());

            message->Parameters().Implementation<IWebServer>()->RemoveProxy(path);
        },
        [](Core::ProxyType<Core::IPCChannel>&, Core::ProxyType<RPC::InvokeMessage>& message) {
            // virtual string Accessor() const = 0;
            string accessorUrl = message->Parameters().Implementation<IWebServer>()->Accessor();
            RPC::Data::Frame::Writer output(message->Response().Writer());
            output.Text(accessorUrl);
        },
        nullptr
    };

    typedef ProxyStub::StubType<IBrowser, BrowserStubMethods, ProxyStub::UnknownStub> BrowserStub;
    typedef ProxyStub::StubType<IBrowser::INotification, BrowserNotificationStubMethods, ProxyStub::UnknownStub> BrowserNotificationStub;
    typedef ProxyStub::StubType<IWebDriver, WebDriverStubMethods, ProxyStub::UnknownStub> WebDriverStub;
    typedef ProxyStub::StubType<IOpenCDMi, OpenCDMiStubMethods, ProxyStub::UnknownStub> OpenCDMiStub;
    typedef ProxyStub::StubType<INetflix, NetflixStubMethods, ProxyStub::UnknownStub> NetflixStub;
    typedef ProxyStub::StubType<INetflix::INotification, NetflixNotificationStubMethods, ProxyStub::UnknownStub> NetflixNotificationStub;
    typedef ProxyStub::StubType<IResourceCenter, ResourceCenterStubMethods, ProxyStub::UnknownStub> ResourceCenterStub;
    typedef ProxyStub::StubType<IResourceCenter::INotification, ResourceCenterNotificationStubMethods, ProxyStub::UnknownStub> ResourceCenterNotificationStub;
    typedef ProxyStub::StubType<IProvisioning, ProvisioningStubMethods, ProxyStub::UnknownStub> ProvisioningStub;
    typedef ProxyStub::StubType<IProvisioning::INotification, ProvisioningNotificationStubMethods, ProxyStub::UnknownStub> ProvisioningNotificationStub;
    typedef ProxyStub::StubType<IWebServer, WebServerStubMethods, ProxyStub::UnknownStub> WebServerStub;
    typedef ProxyStub::StubType<IComposition, CompositionStubMethods, ProxyStub::UnknownStub> CompositionStub;
    typedef ProxyStub::StubType<IComposition::IClient, CompositionClientStubMethods, ProxyStub::UnknownStub> CompositionClientStub;
    typedef ProxyStub::StubType<IComposition::INotification, CompositionNotificationStubMethods, ProxyStub::UnknownStub> CompositionNotificationStub;

    // -------------------------------------------------------------------------------------------
    // PROXY
    // -------------------------------------------------------------------------------------------
    class BrowserProxy : public ProxyStub::UnknownProxyType<IBrowser> {
    public:
        BrowserProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~BrowserProxy()
        {
        }

    public:
        virtual void SetURL(const string& URL)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(URL);
            Invoke(newMessage);
        }
        virtual string GetURL() const
        {
            IPCMessage newMessage(BaseClass::Message(1));
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            return reader.Text();
        }
        virtual uint32 GetFPS() const
        {
            IPCMessage newMessage(BaseClass::Message(2));
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            return reader.Number<uint32>();
        }
        virtual void Register(IBrowser::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(3));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IBrowser::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual void Unregister(IBrowser::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(4));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IBrowser::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual void Hide(const bool hide)
        {
            IPCMessage newMessage(BaseClass::Message(5));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Boolean(hide);
            Invoke(newMessage);
        }
        virtual void Go()
        {
            IPCMessage newMessage(BaseClass::Message(6));
            Invoke(newMessage);
        }
    };

    class BrowserNotificationProxy : public ProxyStub::UnknownProxyType<IBrowser::INotification> {
    public:
        BrowserNotificationProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~BrowserNotificationProxy()
        {
        }

    public:
        virtual void LoadFinished(const string& URL)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(URL);
            Invoke(newMessage);
        }
        virtual void URLChanged(const string& URL)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(URL);
            Invoke(newMessage);
        }
        virtual void Hidden(const bool hidden)
        {
            IPCMessage newMessage(BaseClass::Message(2));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Boolean(hidden);
            Invoke(newMessage);
        }
    };

    class WebDriverProxy : public ProxyStub::UnknownProxyType<IWebDriver> {
    public:
        WebDriverProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~WebDriverProxy()
        {
        }

    public:
        virtual uint32 Configure(PluginHost::IShell* webbridge)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<PluginHost::IShell*>(webbridge);
            Invoke(newMessage);
            return (newMessage->Response().Reader().Number<uint32>());
        }
    };

    class OpenCDMiProxy : public ProxyStub::UnknownProxyType<IOpenCDMi> {
    public:
        OpenCDMiProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~OpenCDMiProxy()
        {
        }

    public:
        virtual uint32 Configure(PluginHost::IShell* webbridge)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<PluginHost::IShell*>(webbridge);
            Invoke(newMessage);
            return (newMessage->Response().Reader().Number<uint32>());
        }
    };

    class NetflixProxy : public ProxyStub::UnknownProxyType<INetflix> {
    public:
        NetflixProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~NetflixProxy()
        {
        }

    public:
        virtual void Register(INetflix::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<INetflix::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual void Unregister(INetflix::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<INetflix::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual string GetESN() const
        {
            IPCMessage newMessage(BaseClass::Message(2));
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            return reader.Text();
        }
        virtual void Go()
        {
            IPCMessage newMessage(BaseClass::Message(3));
            Invoke(newMessage);
        }
        virtual void SystemCommand(const string& command)
        {
            IPCMessage newMessage(BaseClass::Message(4));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(command);
            Invoke(newMessage);
        }
        virtual void Language(const string& language)
        {
            IPCMessage newMessage(BaseClass::Message(5));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(language);
            Invoke(newMessage);
        }
    };

    class NetflixNotificationProxy : public ProxyStub::UnknownProxyType<INetflix::INotification> {
    public:
        NetflixNotificationProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~NetflixNotificationProxy()
        {
        }

    public:
        virtual void StateChange(const Exchange::INetflix::state newState)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<Exchange::INetflix::state>(newState);

            Invoke(newMessage);
        }
    };

    class ResourceCenterProxy : public ProxyStub::UnknownProxyType<IResourceCenter> {
    public:
        ResourceCenterProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~ResourceCenterProxy()
        {
        }

    public:
        virtual uint32 Configure(const string& configuration)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(configuration);
            Invoke(newMessage);
            return (newMessage->Response().Reader().Number<uint32>());
        }
        virtual void Register(IResourceCenter::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IResourceCenter::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual void Unregister(IResourceCenter::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(2));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IResourceCenter::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual Exchange::IResourceCenter::hardware_state HardwareState() const
        {
            IPCMessage newMessage(BaseClass::Message(3));
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            return reader.Number<Exchange::IResourceCenter::hardware_state>();
        }
        virtual uint8 Identifier(const uint8 length, uint8 buffer[]) const
        {
            IPCMessage newMessage(BaseClass::Message(4));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<uint8>(length);
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            uint8 result(reader.Buffer<uint8>(length, buffer));

            return (result > length ? length : result);
        }
    };

    class ResourceCenterNotificationProxy : public ProxyStub::UnknownProxyType<IResourceCenter::INotification> {
    public:
        ResourceCenterNotificationProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~ResourceCenterNotificationProxy()
        {
        }

    public:
        virtual void StateChange(Exchange::IResourceCenter::hardware_state state)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<Exchange::IResourceCenter::hardware_state>(state);
            Invoke(newMessage);
        }
    };

    class ProvisioningProxy : public ProxyStub::UnknownProxyType<IProvisioning> {
    public:
        ProvisioningProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~ProvisioningProxy()
        {
        }

    public:
        virtual void Register(IProvisioning::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IProvisioning::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual void Unregister(IProvisioning::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IProvisioning::INotification*>(notification);
            Invoke(newMessage);
        }
    };

    class ProvisioningNotificationProxy : public ProxyStub::UnknownProxyType<IProvisioning::INotification> {
    public:
        ProvisioningNotificationProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~ProvisioningNotificationProxy()
        {
        }

    public:
        virtual void Provisioned(const string& element)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(element);
            Invoke(newMessage);
        }
    };

    class CompositionProxy : public ProxyStub::UnknownProxyType<IComposition> {
    public:
        CompositionProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~CompositionProxy()
        {
        }

    public:
        virtual void Register(IComposition::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IComposition::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual void Unregister(IComposition::INotification* notification)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IComposition::INotification*>(notification);
            Invoke(newMessage);
        }
        virtual IClient* Client(const uint8 index)
        {
            IPCMessage newMessage(BaseClass::Message(2));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<uint32>(index);
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (CreateProxy<IClient>(reader.Number<IClient*>()));
        }
        virtual IClient* Client(const string& name)
        {
            IPCMessage newMessage(BaseClass::Message(3));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(name);
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (CreateProxy<IClient>(reader.Number<IClient*>()));
        }
    };

    class CompositionClientProxy : public ProxyStub::UnknownProxyType<IComposition::IClient> {
    public:
        CompositionClientProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~CompositionClientProxy()
        {
        }

    public:
        virtual string Name() const
        {
            IPCMessage newMessage(BaseClass::Message(0));
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            return (reader.Text());
        }
        virtual void Kill()
        {
            IPCMessage newMessage(BaseClass::Message(1));
            Invoke(newMessage);
        }
        virtual void Focus()
        {
            IPCMessage newMessage(BaseClass::Message(2));
            Invoke(newMessage);
        }
        virtual void Opacity(const uint32 value)
        {
            IPCMessage newMessage(BaseClass::Message(3));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<uint32>(value);
            Invoke(newMessage);
        }
        virtual void Geometry(const uint32 X, const uint32 Y, const uint32 width, const uint32 height)
        {
            IPCMessage newMessage(BaseClass::Message(4));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<uint32>(X);
            writer.Number<uint32>(Y);
            writer.Number<uint32>(width);
            writer.Number<uint32>(height);
            Invoke(newMessage);
        }
        virtual void Visible(const bool value)
        {
            IPCMessage newMessage(BaseClass::Message(5));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Boolean(value);
            Invoke(newMessage);
        }
        virtual void SetTop()
        {
            IPCMessage newMessage(BaseClass::Message(6));
            Invoke(newMessage);
        }
        virtual void SetInput()
        {
            IPCMessage newMessage(BaseClass::Message(7));
            Invoke(newMessage);
        }
    };

    class CompositionNotificationProxy : public ProxyStub::UnknownProxyType<IComposition::INotification> {
    public:
        CompositionNotificationProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~CompositionNotificationProxy()
        {
        }

    public:
        virtual void Attached(IComposition::IClient* element)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IComposition::IClient*>(element);
            Invoke(newMessage);
        }
        virtual void Detached(IComposition::IClient* element)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<IComposition::IClient*>(element);
            Invoke(newMessage);
        }
    };

    class WebServerProxy : public ProxyStub::UnknownProxyType<IWebServer> {
    public:
        WebServerProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~WebServerProxy()
        {
        }

    public:
        virtual void AddProxy(const string& path, const string& subst, const string& address)
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(path);
            writer.Text(subst);
            writer.Text(address);
            Invoke(newMessage);
        }
        virtual void RemoveProxy(const string& path)
        {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(path);
            Invoke(newMessage);
        }
        virtual string Accessor() const
        {
            IPCMessage newMessage(BaseClass::Message(2));
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            string accessorURL = reader.Text();
            return accessorURL;
        }
    };

    // -------------------------------------------------------------------------------------------
    // Registration
    // -------------------------------------------------------------------------------------------
    static class Instantiation {
    public:
        Instantiation()
        {
            RPC::Administrator::Instance().Announce<IBrowser, BrowserProxy, BrowserStub>();
            RPC::Administrator::Instance().Announce<IBrowser::INotification, BrowserNotificationProxy, BrowserNotificationStub>();
            RPC::Administrator::Instance().Announce<IWebDriver, WebDriverProxy, WebDriverStub>();
            RPC::Administrator::Instance().Announce<IOpenCDMi, OpenCDMiProxy, OpenCDMiStub>();
            RPC::Administrator::Instance().Announce<INetflix, NetflixProxy, NetflixStub>();
            RPC::Administrator::Instance().Announce<INetflix::INotification, NetflixNotificationProxy, NetflixNotificationStub>();
            RPC::Administrator::Instance().Announce<IResourceCenter, ResourceCenterProxy, ResourceCenterStub>();
            RPC::Administrator::Instance().Announce<IResourceCenter::INotification, ResourceCenterNotificationProxy, ResourceCenterNotificationStub>();
            RPC::Administrator::Instance().Announce<IProvisioning, ProvisioningProxy, ProvisioningStub>();
            RPC::Administrator::Instance().Announce<IProvisioning::INotification, ProvisioningNotificationProxy, ProvisioningNotificationStub>();
            RPC::Administrator::Instance().Announce<IWebServer, WebServerProxy, WebServerStub>();
            RPC::Administrator::Instance().Announce<IComposition, CompositionProxy, CompositionStub>();
            RPC::Administrator::Instance().Announce<IComposition::IClient, CompositionClientProxy, CompositionClientStub>();
            RPC::Administrator::Instance().Announce<IComposition::INotification, CompositionNotificationProxy, CompositionNotificationStub>();
        }
        ~Instantiation()
        {
        }

    } ProxyStubRegistration;
}
}
