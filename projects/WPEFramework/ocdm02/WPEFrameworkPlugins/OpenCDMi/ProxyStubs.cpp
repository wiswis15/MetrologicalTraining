#include "Module.h"
#include <ocdm/IOCDM.h>

// TODO: shouldn't all these lambda-arrays be static?
namespace WPEFramework {

    // -------------------------------------------------------------------------------------------
    // STUB
    // -------------------------------------------------------------------------------------------

    //
    // OCDM::IAccessor interface stub definitions (interface/ICDM.h)
    //
    ProxyStub::MethodHandler AccesorStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            fprintf(stderr, "%s:%d In first (IsTypeSupported) lambda\n", __FILE__, __LINE__);
            //
            // virtual WPEFramework::Core::Error IsTypeSupported(
            //     const std::string keySystem,
            //     const std::string mimeType) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            std::string keySystem = parameters.Text();
            std::string mimeType = parameters.Text();
            
            WPEFramework::Core::Error returnedValue = message->Parameters().Implementation<OCDM::IAccessor>()->IsTypeSupported(keySystem, mimeType);

            response.Number(returnedValue);
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // ISystem * CreateSystem(
            //      const std::string& keySystem) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            std::string keySystem = parameters.Text();

            // TODO: what about impl/proxies?
            response.Number(message->Parameters().Implementation<OCDM::IAccessor>()->CreateSystem(keySystem));
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // ISession* Session(
            //        const uint8_t keyId[],
            //        const uint8_t length,
            //        const uint32_t maxWaitTime) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            const uint8_t* keyId = nullptr;
            uint8_t keyIdLength = parameters.LockBuffer<uint8_t>(keyId);
            parameters.UnlockBuffer(keyIdLength);

            uint32_t maxWaitTime = parameters.Number<uint32_t>();

            response.Number(message->Parameters().Implementation<OCDM::IAccessor>()->Session(keyId, keyIdLength, maxWaitTime));
        },
        nullptr
    };

    //
    // OCDM::ISession::ICallback interface stub definitions (interface/ICDM.h)
    //
    ProxyStub::MethodHandler CallbackStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual void ProcessChallenge(
            //    const std::string& URL,
            //    const uint8_t challenge[],
            //    const uint16_t challengeLength) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string URL = parameters.Text();
            const uint8_t* challenge = nullptr;
            uint16_t challengeLength = parameters.LockBuffer<uint16_t>(challenge);
            parameters.UnlockBuffer(challengeLength);

            message->Parameters().Implementation<OCDM::ISession::ICallback>()->ProcessChallenge(URL, challenge, challengeLength);
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual void KeyUpdate(
            //    const uint8_t keyId[],
            //    const uint8_t length) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            string URL = parameters.Text();
            const uint8_t* keyId = nullptr;
            uint16_t length = parameters.LockBuffer<uint16_t>(keyId);
            parameters.UnlockBuffer(length);

            message->Parameters().Implementation<OCDM::ISession::ICallback>()->KeyUpdate(keyId, length);
        },
        nullptr
    };

    //
    // OCDM::ISession interface stub definitions (interface/ICDM.h)
    //
    ProxyStub::MethodHandler SessionStubMethods[] = {
       [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual WPEFramework::Core::Error Load() = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());
 
            response.Number(message->Parameters().Implementation<OCDM::ISession>()->Load());
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual void Update(
            //    const uint8_t keyMessage[],
            //    const uint16_t keyLength) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());

            const uint8_t* keyMessage = nullptr;
            uint16_t keyLength = parameters.LockBuffer<uint16_t>(keyMessage);
 
            message->Parameters().Implementation<OCDM::ISession>()->Update(keyMessage, keyLength);
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual WPEFramework::Core::Error Remove() = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());
 
            response.Number(message->Parameters().Implementation<OCDM::ISession>()->Remove());
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual KeyStatus Status(
            //    const uint8_t keyId[],
            //    const uint8_t length) const = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            const uint8_t* keyId = nullptr;
            uint16_t length = parameters.LockBuffer<uint16_t>(keyId);

            response.Number(message->Parameters().Implementation<OCDM::ISession>()->Status(keyId, length));
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual uint32_t Error(
            //    const uint8_t keyId[],
            //    const uint8_t length) const = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            const uint8_t* keyId = nullptr;
            uint16_t length = parameters.LockBuffer<uint16_t>(keyId);

            response.Number(message->Parameters().Implementation<OCDM::ISession>()->Error(keyId, length));
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual WPEFramework::Core::Error SystemError() const = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            response.Number(message->Parameters().Implementation<OCDM::ISession>()->SystemError());
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual std::string BufferId() const = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());
 
            response.Text(message->Parameters().Implementation<OCDM::ISession>()->BufferId());
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual std::string SessionId() const = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());
 
            response.Text(message->Parameters().Implementation<OCDM::ISession>()->SessionId());
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual void Close() = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());
 
            message->Parameters().Implementation<OCDM::ISession>()->Close();
        },
        [](Core::ProxyType<Core::IPCChannel>& channel, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual WPEFramework::Core::Error Callback(
            //    ICallback* callback) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            OCDM::ISession::ICallback* implementation = parameters.Number<OCDM::ISession::ICallback*>();
            OCDM::ISession::ICallback* proxy = nullptr;

            if (implementation != nullptr) {
                proxy = RPC::Administrator::Instance().CreateProxy<OCDM::ISession::ICallback>(channel, implementation, true, false);

                ASSERT((proxy != nullptr) && "Failed to create proxy");
            }

            WPEFramework::Core::Error result = message->Parameters().Implementation<OCDM::ISession>()->Callback(proxy);

            if ( (proxy != nullptr) && (proxy->Release() != Core::ERROR_NONE)) {
                TRACE_L1("Oops seems like we did not maintain a reference to this sink. %d", __LINE__);
            }

            response.Number(result);
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual WPEFramework::Core::Error Decrypt(
            //    uint8_t encrypted[],
            //    const uint32_t encryptedLength,
            //    const uint8_t IV[],
            //    const uint16_t IVLength) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            const uint8_t* encrypted = nullptr;
            uint32_t encryptedLength = parameters.LockBuffer<uint32_t>(encrypted);
            const uint8_t* IV = nullptr;
            uint16_t IVLength = parameters.LockBuffer<uint16_t>(IV);

            // TODO: here stuff with SharedBuffers etc? ("DataExchange"?)
            // TODO: "encrypted" is in/out parm (right?) needs to send back data
            // HACK
            uint8_t* hackNonConstEncrypted = const_cast<uint8_t *>(encrypted);
            response.Number(message->Parameters().Implementation<OCDM::ISession>()->Decrypt(hackNonConstEncrypted, encryptedLength, IV, IVLength));
        },
        nullptr
    };
    
    //
    // OCDM::ISystem interface stub definitions
    //
    ProxyStub::MethodHandler SystemStubMethods[] = {
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual WPEFramework::Core::Error SetServerCertificate(
            //    const uint8_t serverCertificate[],
            //    const uint16_t serverCertificateLength) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            const uint8_t* serverCertificate = nullptr;
            uint16_t serverCertificateLength = parameters.LockBuffer<uint16_t>(serverCertificate);
 
            response.Number(message->Parameters().Implementation<OCDM::ISystem>()->SetServerCertificate(serverCertificate, serverCertificateLength));
        },
        [](Core::ProxyType<Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Core::ProxyType<RPC::InvokeMessage>& message) {
            //
            // virtual WPEFramework::Core::Error CreateSession(
            //    const int32_t licenseType,
            //    const std::string& initDataType,
            //    const uint8_t initData[],
            //    const uint16_t initDataLength,
            //    const uint8_t* CDMData,
            //    const uint16_t CDMDataLength,
            //    ISession*& session) = 0;
            //
            RPC::Data::Frame::Reader parameters(message->Parameters().Reader());
            RPC::Data::Frame::Writer response(message->Response().Writer());

            int32_t licenseType = parameters.Number<int32_t>();
            std::string initDataType = parameters.Text();
            const uint8_t* initData = nullptr;
            uint16_t initDataLength = parameters.LockBuffer<uint16_t>(initData);
            const uint8_t* CDMData = nullptr;
            uint16_t CDMDataLength = parameters.LockBuffer<uint16_t>(CDMData);
            OCDM::ISession* session = nullptr;

            fprintf(stderr, __FILE__ " In CreateSession handler, writing two numbers.\n");

            // TODO: what about impl/proxies?
            response.Number(message->Parameters().Implementation<OCDM::ISystem>()->CreateSession(licenseType, initDataType, initData, initDataLength, CDMData, CDMDataLength, session));

            // TODO: guess we need to return "session" here?
            response.Number(session);
        },
        nullptr
    };


    typedef ProxyStub::StubType<OCDM::IAccessor, AccesorStubMethods, ProxyStub::UnknownStub> AccessorStub;
    typedef ProxyStub::StubType<OCDM::ISession::ICallback, CallbackStubMethods, ProxyStub::UnknownStub> CallbackStub;
    typedef ProxyStub::StubType<OCDM::ISession, SessionStubMethods, ProxyStub::UnknownStub> SessionStub;
    typedef ProxyStub::StubType<OCDM::ISystem, SystemStubMethods, ProxyStub::UnknownStub> SystemStub;

    // -------------------------------------------------------------------------------------------
    // PROXY
    // -------------------------------------------------------------------------------------------
    class AccessorProxy : public ProxyStub::UnknownProxyType<OCDM::IAccessor> {
    public:
        AccessorProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation,
            const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~AccessorProxy()
        {
        }

    public:
        virtual WPEFramework::Core::Error IsTypeSupported(
           const std::string& keySystem,
           const std::string& mimeType) const override {
           
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(keySystem);
            writer.Text(mimeType);
 
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<WPEFramework::Core::Error>());
        }
        virtual OCDM::ISystem * CreateSystem(
            const string& keySystem) override {

            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(keySystem);

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            OCDM::ISystem* result = reader.Number<OCDM::ISystem*>();

            return (result);
        }
        virtual OCDM::ISession* Session (
            const uint8_t keyId[],
            const uint8_t keyIdLength,
            const uint32_t maxWaitTime) override {

            IPCMessage newMessage(BaseClass::Message(2));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Buffer(keyIdLength, keyId);
            writer.Number(maxWaitTime);
 
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (CreateProxy<OCDM::ISession>(reader.Number<OCDM::ISession*>()));
        }
    };
    
    class SystemProxy : public ProxyStub::UnknownProxyType<OCDM::ISystem> {
    public:
        SystemProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
          : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~SystemProxy()
        {
        }
    public:
        virtual WPEFramework::Core::Error SetServerCertificate(const uint8_t serverCertificate[], const uint16_t serverCertificateLength) override
        {

            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Buffer(serverCertificateLength, serverCertificate);

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<WPEFramework::Core::Error>());
        }

        virtual WPEFramework::Core::Error CreateSession(const int32_t licenseType,
                                     const std::string& initDataType, const uint8_t initData[], const uint16_t initDataLength,
                                     const uint8_t* CDMData, const uint16_t CDMDataLength,
                                     OCDM::ISession*& session) override
        {
            // TODO: make sure "session" is correctly returned

            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number(licenseType);
            writer.Text(initDataType);
            writer.Buffer(initDataLength, initData);
            writer.Buffer(CDMDataLength, CDMData);

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
            WPEFramework::Core::Error result = reader.Number<WPEFramework::Core::Error>();

            session = reader.Number<OCDM::ISession*>();
            return result;
        }
    };

    class CallbackProxy : public ProxyStub::UnknownProxyType<OCDM::ISession::ICallback> {
    public:
        CallbackProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }

        virtual ~CallbackProxy()
        {
        }

    public:
        virtual void ProcessChallenge(const std::string& URL, const uint8_t challenge[], const uint16_t challengeLength) override
        {
            IPCMessage newMessage(BaseClass::Message(0));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Text(URL);
            writer.Buffer(challengeLength, challenge);
            Invoke(newMessage);
        }

        virtual void KeyUpdate(const uint8_t keyId[], const uint8_t length) override
        {
            fprintf(stderr, "%s:%d KeyUpdate: %d\n", __FILE__, __LINE__, length);

            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Buffer(length, keyId);
            Invoke(newMessage);
        }
    };

    // TODO: remove this comment
    // override SessionProxy
    class SessionProxy : public ProxyStub::UnknownProxyType<OCDM::ISession> {
    public:
        SessionProxy(Core::ProxyType<Core::IPCChannel>& channel, void* implementation,
            const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }
        virtual ~SessionProxy()
        {
        }

    public:
        //
        // Loads the data stored for the specified session into the cdm object
        //
        virtual WPEFramework::Core::Error Load() override {
            IPCMessage newMessage(BaseClass::Message(0));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<WPEFramework::Core::Error>());
        }
        //
        // Process a key message response.
        //
        virtual void Update(const uint8_t* keyMessage, const uint16_t keyLength) override {
            IPCMessage newMessage(BaseClass::Message(1));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Buffer(keyLength, keyMessage);

            Invoke(newMessage);
        } 
        //
        // Removes all license(s) and key(s) associated with the session
        //
        virtual WPEFramework::Core::Error Remove() override {
            IPCMessage newMessage(BaseClass::Message(2));

            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<WPEFramework::Core::Error>());
        }
        //
        // report the current status of the Session with respect to the KeyExchange.
        //
        virtual OCDM::ISession::KeyStatus Status(const uint8_t keyId[], const uint8_t length) const override {

            IPCMessage newMessage(BaseClass::Message(3));
 
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Buffer(length, keyId);
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<OCDM::ISession::KeyStatus>());
        }
        virtual uint32_t Error(const uint8_t keyId[], const uint8_t length) const override {

            IPCMessage newMessage(BaseClass::Message(4));
 
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Buffer(length, keyId);
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<uint32_t>());
        }
        virtual WPEFramework::Core::Error SystemError() const override {
            IPCMessage newMessage(BaseClass::Message(5));
 
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<WPEFramework::Core::Error>());
        }
        virtual std::string BufferId() const override {

            IPCMessage newMessage(BaseClass::Message(6));
 
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual std::string SessionId() const override {

            IPCMessage newMessage(BaseClass::Message(7));
 
            Invoke(newMessage);

            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Text());
        }
        virtual void Close () override {

            IPCMessage newMessage(BaseClass::Message(8));
 
            Invoke(newMessage);
        }
        virtual WPEFramework::Core::Error Callback(ICallback* callback) override {
        
            IPCMessage newMessage(BaseClass::Message(9));
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            
            writer.Number(callback);
 
            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<WPEFramework::Core::Error>());
        }
        virtual WPEFramework::Core::Error Decrypt(uint8_t encrypted[], const uint32_t encryptedLength, const uint8_t IV[], const uint16_t IVLength) override {

            IPCMessage newMessage(BaseClass::Message(10));

            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());

            // TODO: SharedBuffer, right?
            writer.Buffer(encryptedLength, encrypted);
            writer.Buffer(IVLength, IV);

            Invoke(newMessage);
            RPC::Data::Frame::Reader reader(newMessage->Response().Reader());

            return (reader.Number<WPEFramework::Core::Error>());
        }
    };
 
    // -------------------------------------------------------------------------------------------
    // These proxy stubs are "loaded" by the next method, which needs to be explicitely called
    // since the interface is a dedicated interface and needs loading, if required by a 
    // wrapper around the interface. 
    // -------------------------------------------------------------------------------------------
    static class Instantiation {
    public:
        Instantiation()
        {
            fprintf(stderr, "Registering OCDM handlers\n");
            RPC::Administrator::Instance().Announce<OCDM::ISession::ICallback, CallbackProxy, CallbackStub>();
            RPC::Administrator::Instance().Announce<OCDM::ISession, SessionProxy, SessionStub>();
            RPC::Administrator::Instance().Announce<OCDM::IAccessor, AccessorProxy, AccessorStub>();
            RPC::Administrator::Instance().Announce<OCDM::ISystem, SystemProxy, SystemStub>();
        }

        ~Instantiation()
        {
        }

    } OCDMProxyStubRegistration;

} // namespace WPEFramework
