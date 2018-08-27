#include "Module.h"

#include "FrameworkRPC.h"

// Get in the definitions required for access to the specific
// DRM engines.
#include <interfaces/IDRM.h>

// Get in the definitions required for access to the OCDM 
// counter part living in the applications
#include <ocdm/IOCDM.h>
#include <ocdm/DataExchange.h>

#include <interfaces/IContentDecryption.h>

#include "CENCParser.h"

extern "C" {

typedef CDMi::ISystemFactory* (*GetDRMSystemFunction)();

}

using CDMi::CDMi_RESULT;

// TODO: Check these
static WPEFramework::Core::Error CdmiResultToWpeError(CDMi::CDMi_RESULT result)
{
    if (CDMi_SUCCEEDED(result))
        return WPEFramework::Core::Error::ERROR_NONE;

    WPEFramework::Core::Error output = WPEFramework::Core::Error::ERROR_GENERAL;
    switch(result) {
        case CDMi_E_FAIL:
            output = WPEFramework::Core::Error::ERROR_GENERAL;
            break;
        case CDMi_E_INVALID_ARG:
            output = WPEFramework::Core::Error::ERROR_BAD_REQUEST;
            break;
        case CDMi_E_OUT_OF_MEMORY:
            output = WPEFramework::Core::Error::ERROR_GENERAL;
            break;
        case CDMi_E_SERVER_INTERNAL_ERROR:
            output = WPEFramework::Core::Error::ERROR_GENERAL;
            break;
        case CDMi_E_SERVER_INVALID_MESSAGE:
            output = WPEFramework::Core::Error::ERROR_GENERAL;
            break;
        case CDMi_E_SERVER_SERVICE_SPECIFIC:
            output = WPEFramework::Core::Error::ERROR_GENERAL;
            break;
        case CDMi_S_FALSE:
            output = WPEFramework::Core::Error::ERROR_GENERAL;
            break;
        case CDMi_SUCCESS:
            output = WPEFramework::Core::Error::ERROR_NONE;
            break;
    }

    return output;
}

namespace WPEFramework {

namespace Plugin {

    static const TCHAR BufferFileName[] = _T("ocdmbuffer.");

    class OCDMImplementation : public Exchange::IContentDecryption {
    private:
        OCDMImplementation(const OCDMImplementation&) = delete;
        OCDMImplementation& operator=(const OCDMImplementation&) = delete;

        struct SystemFactory {
            std::string Name;
            CDMi::ISystemFactory* Factory;
        };
 
        class ExternalAccess : public RPC::Communicator
        {
        private:
            ExternalAccess() = delete;
            ExternalAccess(const ExternalAccess &) = delete;
            ExternalAccess & operator=(const ExternalAccess &) = delete;

            class RequestHandler : public Core::IPCServerType<RPC::InvokeMessage>, public Core::Thread
            {
            private:
                struct Info
                {
                    Core::ProxyType<RPC::InvokeMessage> message;
                    Core::ProxyType<Core::IPCChannel> channel;
                };
                typedef Core::QueueType<Info> MessageQueue;

                RequestHandler(const RequestHandler &) = delete;
                RequestHandler & operator=(const RequestHandler &) = delete;

            public:
                RequestHandler()
                    : Core::IPCServerType<RPC::InvokeMessage>(), Core::Thread(), _handleQueue(64),
                      _handler(RPC::Administrator::Instance())
                {
                    Run();
                }

                ~RequestHandler()
                {
                    Thread::Stop();
                    _handleQueue.Disable();
                    Thread::Wait(Thread::BLOCKED | Thread::STOPPED, Core::infinite);
                 }

            public:
                virtual void Procedure(Core::IPCChannel & channel, Core::ProxyType<RPC::InvokeMessage> & data)
                {
                    // Oke, see if we can reference count the IPCChannel
                    Info newElement;
                    newElement.channel = Core::ProxyType<Core::IPCChannel>(channel);
                    newElement.message = data;

                    ASSERT(newElement.channel.IsValid() == true);

                    _handleQueue.Insert(newElement, Core::infinite);
                }

                virtual uint32_t Worker()
                {
                    Info newRequest;

                    while (_handleQueue.Extract(newRequest, Core::infinite) == true)
                    {

                        _handler.Invoke(newRequest.channel, newRequest.message);

                        Core::ProxyType<Core::IIPC> message(newRequest.message);
                        newRequest.channel->ReportResponse(message);
                    }

                    return (Core::infinite);
                }

            private:
                MessageQueue _handleQueue;
                RPC::Administrator & _handler;
            };

            class ObjectMessageHandler : public Core::IPCServerType<RPC::ObjectMessage>
            {
            private:
                ObjectMessageHandler() = delete;
                ObjectMessageHandler(const ObjectMessageHandler &) = delete;
                ObjectMessageHandler & operator=(const ObjectMessageHandler &) = delete;

            public:
                ObjectMessageHandler(::OCDM::IAccessor* parentInterface)
                    : _parentInterface(parentInterface)
                {
                }
                ~ObjectMessageHandler()
                {
                }

            public:
                virtual void Procedure(Core::IPCChannel & channel, Core::ProxyType<RPC::ObjectMessage> & data)
                {
                    // Oke, see if we can reference count the IPCChannel
                    Core::ProxyType<Core::IPCChannel> refChannel(channel);

                    ASSERT(refChannel.IsValid());
 
                    if (refChannel.IsValid() == true)
                    {
                        const uint32_t interfaceId(data->Parameters().InterfaceId());
                        const uint32_t versionId(data->Parameters().VersionId());

                        // Currently we only support version 1 of the IRPCLink :-)
                        if (((versionId == 1) || (versionId == static_cast<uint32_t>(~0))) &&
                            (interfaceId == ::OCDM::IAccessor::ID)) {
                            // Reference count our parent
                            _parentInterface->AddRef();

                            // Allright, respond with the interface.
                            data->Response().Value(_parentInterface);
                        }
                        else {
                            // Allright, respond with the interface.
                            data->Response().Value(nullptr);
                         }
                    }

                    Core::ProxyType<Core::IIPC> returnValue(data);
                    channel.ReportResponse(returnValue);
                }

            private:
                ::OCDM::IAccessor* _parentInterface;
            };

        public:
            ExternalAccess(const Core::NodeId & source, ::OCDM::IAccessor* parentInterface)
                : RPC::Communicator(source, Core::ProxyType<RequestHandler>::Create())
                , _handler(Core::ProxyType<ObjectMessageHandler>::Create(parentInterface))
            {
                RPC::Communicator::CreateFactory<RPC::ObjectMessage>(1);
                RPC::Communicator::Register(_handler);
            }

            ~ExternalAccess()
            {
                Close(Core::infinite);
                RPC::Communicator::Unregister(_handler);
                RPC::Communicator::DestroyFactory<RPC::ObjectMessage>();
            }

        private:
            Core::ProxyType<ObjectMessageHandler> _handler;
        };

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            class Link : public Core::JSON::Container {
            private:
                Link& operator= (const Link&);

            public:
                Link ()
                    : Key()
                    , System(){
                    Add("key", &Key);
                    Add("system", &System);
                }
                Link (const Link& copy)
                    : Key(copy.Key)
                    , System(copy.System){
                    Add("key", &Key);
                    Add("system", &System);
                }
                virtual ~Link() {
                }

            public:
                Core::JSON::String Key;
                Core::JSON::String System;
            };

        public:
            Config()
                : Core::JSON::Container()
                , Location()
                , Connector(_T("/tmp/ocdm"))
                , SharePath(_T("/tmp"))
                , ShareSize(8 * 1024)
                , Mapping()
            {
                Add(_T("location"), &Location);
                Add(_T("connector"), &Connector);
                Add(_T("sharepath"), &SharePath);
                Add(_T("sharesize"), &ShareSize);
                Add(_T("mapping"), &Mapping);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::String Location;
            Core::JSON::String Connector;
            Core::JSON::String SharePath;
            Core::JSON::DecUInt32 ShareSize;
            Core::JSON::ArrayType<Link> Mapping;
        };

    public:
        OCDMImplementation()
            : _entryPoint(nullptr)
            , _service(nullptr)
            , _compliant(false)
            , _systemToFactory()
            , _systemLibraries()
        {
            TRACE_L1("Constructing OCDMImplementation Service: %p", this);
        }
        virtual ~OCDMImplementation()
        {
            if (_service != nullptr) {
                delete _service;
            }

            if (_entryPoint != nullptr) {
                _entryPoint->Release();
            }

            _systemLibraries.clear();

            TRACE_L1("Destructed OCDMImplementation Service: %p", this);
        }

    public:
        virtual uint32_t Configure(PluginHost::IShell* service)
        {
            uint32_t result = Core::ERROR_OPENING_FAILED;

            // On activation subscribe, on deactivation un-subscribe
            PluginHost::ISubSystem* subSystem = service->SubSystems();

            ASSERT(subSystem != nullptr);

            // Start loading the configured factories
            Config config;
            config.FromString(service->ConfigLine());

            const string locator (service->DataPath() + config.Location.Value());

            // Before we start loading the mapping of the Keys to the factories, load the factories :-)
            Core::Directory entry(locator.c_str(), _T("*.drm"));
            std::map<const string, SystemFactory> factories;

            while (entry.Next() == true) {
                Core::Library library(entry.Current().c_str());

                if (library.IsLoaded() == true) {
                    GetDRMSystemFunction handle = reinterpret_cast<GetDRMSystemFunction>(library.LoadFunction(_T("GetSystemFactory")));

                    if (handle != nullptr) {
                        CDMi::ISystemFactory* entry = handle();

                        if (handle != nullptr) {
                            SystemFactory element;
                            element.Name = Core::ClassNameOnly(entry->KeySystem()).Text();
                            element.Factory = entry;
                            _keySystems.push_back(element.Name);
                            factories.insert(std::pair<const string, SystemFactory>(element.Name, element));
                            _systemLibraries.push_back(library);
                        }
                    }
                }
            }

            Core::JSON::ArrayType< Config::Link >::ConstIterator index (static_cast<const Config&>(config).Mapping.Elements());

            while (index.Next () == true) {

                const string system (index.Current().System.Value());

                if ( (system.empty() == false) && (index.Current().Key.Value().empty() == false) ) {
                    // Find a factory for the key system:
                    std::map<const string, SystemFactory>::iterator factory (factories.find(system));

                    if (factory != factories.end()) {
                        // Register this handler
                        _systemToFactory.insert(std::pair<const std::string, SystemFactory>(index.Current().Key.Value(), factory->second));
                    }
                    else {
                        SYSLOG(PluginHost::Startup, (_T("Required factory [%s], not found for [%s]"), system.c_str(), index.Current().Key.Value().c_str()));
                    }
                }
            }

            if (_systemToFactory.size() == 0) {
                SYSLOG(PluginHost::Startup, (_T("No DRM factories specified. OCDM can not service any DRM requests.")));
            }

            _entryPoint = Core::Service<AccessorOCDM>::Create<::OCDM::IAccessor>(this, config.SharePath.Value(), config.ShareSize.Value());
            _service = new ExternalAccess(Core::NodeId(config.Connector.Value().c_str()), _entryPoint);

            if (_service != nullptr) {

                result = _service->Open(Core::infinite);

                if (result != Core::ERROR_NONE) {
                    delete _service;
                    _entryPoint->Release();
                    _service = nullptr;
                    _entryPoint = nullptr;
                }
                else {
                    if (subSystem != nullptr) {

                        ASSERT (subSystem->IsActive(PluginHost::ISubSystem::DECRYPTION) == false);
                        subSystem->Set(PluginHost::ISubSystem::DECRYPTION, this);
                    }
                    if (_systemToFactory.size() == 0) {
                        SYSLOG(PluginHost::Startup, (string(_T("OCDM server has NO key systems registered!!!"))));
                    }
                }

            }
            return (result);
        }
        virtual uint32_t Reset() {
            return (Core::ERROR_NONE);
        }
        virtual RPC::IStringIterator* Systems() const {
            return (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(_keySystems));
        }
        virtual RPC::IStringIterator* Designators(const string& keySystem) const {
            std::list<string> designators;
            LoadDesignators(keySystem, designators);
            return (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(designators));
        }
        virtual RPC::IStringIterator* Sessions(const string& keySystem) const {
            std::list<string> sessions;
            LoadSessions(keySystem, sessions);
            return (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(sessions));
        }

    public:
        bool IsTypeSupported(const std::string& keySystem, const std::string& mimeType) {

            // FIXME: The dead code below this statement has at least the following issues,
            //  - The MIME checking code is **very far** from compliant (no codec= support is one major issue, there are more)
            //  - If it's for HTML5 user agents, it's pointless because they do their own
            //    compliance checking for MIME types.
            //  - If it's for the out-of-app media engines, then interfaces that talk to
            //    those engines should be provided to check this properly.
            //  - Iterating the keys of a map and then strcmp'ing them isn't how maps are
            //    supposed to be used. The code below may as well have chosen a vector.
            //    map[string] is a much better way to do that, although when it's unlikely
            //    to ever have more than 3 keys, a map is pretty pointless as well.
            //  So given the above, what I've changed here is that now we just check if
            //  the key system is supported, because that's something we can easily do
            //  and that check belongs in our domain.
            bool result = (keySystem.empty() == false);

            if (result == true) {
                std::map<const std::string, SystemFactory>::iterator index (_systemToFactory.find(keySystem));

                if (index == _systemToFactory.end()) {
                    result = false;
                }
                else {
                    //const std::vector<std::string>& mimes (index->second->MimeTypes());
            //mime type still needs to be parsed.
                    result = true;//((mimeType.empty() == true) || std::find(mimes.begin(), mimes.end(), mimeType) != mimes.end());
                }
            }

            TRACE(Trace::Information, ("IsTypeSupported(%s,%s) => %s", keySystem.c_str(), mimeType.c_str(), result ? _T("True") : _T("False")));

            return result;
        }

        CDMi::IMediaKeys* KeySystem(const std::string& keySystem) {
            CDMi::IMediaKeys* result = nullptr;

            if (keySystem.empty() == false) {
                std::map<const std::string, SystemFactory>::iterator index (_systemToFactory.find(keySystem));

                if (_systemToFactory.end() != index) {
                    result = index->second.Factory->Instance();
                }
            }

            TRACE(Trace::Information, ("KeySystem(%s) => %p", keySystem.c_str(), result));
            return (result);
        }

    private:
        void LoadDesignators(const string& keySystem, std::list<string>& designators) const {
            std::map<const std::string, SystemFactory>::const_iterator index (_systemToFactory.begin());
            while (index != _systemToFactory.end()) {
                if (keySystem == index->second.Name) {
                    designators.push_back(index->first);
                }
                index++;
            }
        }
        void LoadSessions(const string& keySystem, std::list<string>& designators) const {
            std::map<const std::string, SystemFactory>::const_iterator index (_systemToFactory.begin());
            while (index != _systemToFactory.end()) {
                if (keySystem == index->second.Name) {
                    designators.push_back(index->first);
                }
                index++;
            }
        }

    private:
        // -------------------------------------------------------------------------------------------------------------
        // IDecryption methods
        // -------------------------------------------------------------------------------------------------------------
        BEGIN_INTERFACE_MAP(OCDMImplementation)
            INTERFACE_ENTRY(Exchange::IContentDecryption)
        END_INTERFACE_MAP

    private:
        ::OCDM::IAccessor* _entryPoint;
        ExternalAccess* _service;
        bool _compliant;
        std::map<const std::string,SystemFactory> _systemToFactory;
        std::list<Core::Library> _systemLibraries;
        std::list<string> _keySystems;
    };

//////////////////////////////////////////////////////////////////////////////
// Start of implementations of AccessorOCDM and nested classes's methods
//////////////////////////////////////////////////////////////////////////////

AccessorOCDM::BufferAdministrator::BufferAdministrator(const string& pathName) :
        _adminLock(), _basePath(Core::Directory::Normalize(pathName)), _occupation(0)
{
}

AccessorOCDM::BufferAdministrator::~BufferAdministrator()
{
}

bool AccessorOCDM::BufferAdministrator::AquireBuffer(string& locator)
{
    uint8_t index = 0;

    locator.clear();

    _adminLock.Lock();

    while ((index < (sizeof(_occupation) * 8)) && ((_occupation & (1 << index)) != 0)) {
        index++;
    }

    // We do not expect more that 4 buffers to be allocated concurrently,
    // so more than X would be dramatic
    ASSERT(index < (sizeof(_occupation) * 8));

    if (index < (sizeof(_occupation) * 8)) {
        _occupation |= (1 << index);
        locator = _basePath + BufferFileName + Core::NumberType<uint8_t>(index).Text();
    }

    _adminLock.Unlock();

    return (locator.empty() == false);
}

bool AccessorOCDM::BufferAdministrator::ReleaseBuffer(const string& locator)
{
    bool released = false;

    if (locator.compare(0, _basePath.length(), _basePath) == 0) {
        string actualFile(locator.substr(_basePath.length()));
        uint8_t baseLength((sizeof(BufferFileName) / sizeof(TCHAR)) - 1);

        if (actualFile.compare(0, baseLength, BufferFileName) == 0) {
            // Than the last part is the number..
            uint8_t number(Core::NumberType<uint8_t>(&(actualFile.c_str()[baseLength]), actualFile.length() - baseLength).Value());

            if (number <= (sizeof(_occupation) * 8)) {
                _adminLock.Lock();

                if ((_occupation & (1 << number)) != 0) {
                    _occupation ^= (1 << number);
                    released = true;
                } else {
                    // Freeing a buffer that is already free sounds dangerous !!!
                    ASSERT(false);
                }

                _adminLock.Unlock();

            }
        }
    }
    return (released);
}

AccessorOCDM::SessionImplementation::DataExchange::DataExchange(CDMi::IMediaKeySession* mediaKeys,
        const string& name, const uint32_t defaultSize) :
            OCDM::DataExchange(name, defaultSize),
            Core::Thread(Core::Thread::DefaultStackSize(), _T("DRMSessionThread")),
            _mediaKeys(mediaKeys),
            _sessionKey(nullptr),
            _sessionKeyLength(0)
{
    Core::Thread::Run();
    TRACE_L1("Constructing buffer server side: %p - %s", this, name.c_str());
    fprintf(stderr, "Created AccessorOCDM, default size: %u\n", defaultSize);
}

AccessorOCDM::SessionImplementation::DataExchange::~DataExchange()
{
    TRACE_L1("Destructing buffer server side: %p - %s", this, OCDM::DataExchange::Name().c_str());
    // Make sure the thread reaches a HALT.. We are done.
    Core::Thread::Stop();

    // If the thread is waiting for a semaphore, fake a signal :-)
    Produced();

    Core::Thread::Wait(Core::Thread::STOPPED, Core::infinite);
}

uint32_t AccessorOCDM::SessionImplementation::DataExchange::Worker()
{
    while (IsRunning() == true) {

        uint32_t clearContentSize = 0;
        uint8_t* clearContent = nullptr;

        RequestConsume(WPEFramework::Core::infinite);

        if (IsRunning() == true) {
            int cr = _mediaKeys->Decrypt(_sessionKey, _sessionKeyLength, nullptr,       //subsamples
                    0,          //number of subsamples
                    IVKey(), IVKeyLength(), Buffer(), BytesWritten(), &clearContentSize, &clearContent);

            if ((cr == 0) && (clearContentSize != 0)) {
                if (clearContentSize != BytesWritten()) {
                    TRACE_L1("Returned clear sample size (%d) differs from encrypted buffer size (%d)", clearContentSize, BytesWritten());
                    Size(clearContentSize);
                }

                // Adjust the buffer on our sied (this process) on what we will write back
                SetBuffer(0, clearContentSize, clearContent);
            }

            // Whatever the result, we are done with the buffer..
            Consumed();
        }
    }

    return (Core::infinite);
}

AccessorOCDM::SessionImplementation::Sink::Sink(SessionImplementation* parent) :
            _parent(*parent),
            _callback(nullptr)
{
}

AccessorOCDM::SessionImplementation::Sink::~Sink()
{
    if (_callback != nullptr) {
        _callback->Release();
    }
}

void AccessorOCDM::SessionImplementation::Sink::OnKeyMessage(const uint8_t *f_pbKeyMessage, uint32_t f_cbKeyMessage, char *f_pszUrl)
{
    TRACE(Trace::Information, ("OnKeyMessage(%s)", f_pszUrl));
    if (_callback != nullptr) {
        std::string url(f_pszUrl, strlen(f_pszUrl));
        _callback->ProcessChallenge(url, f_pbKeyMessage, f_cbKeyMessage);
    } else {
        WaitingMessage message = WaitingMessage::ConstructChallenge(f_pbKeyMessage, f_cbKeyMessage, f_pszUrl);
        _WaitingMessages.push_back(message);
    }
}

void AccessorOCDM::SessionImplementation::Sink::OnKeyReady()
{
    TRACE(Trace::Information, ("OnKeyReady()"));
    if (_callback != nullptr) {
        //_callback->OnKeyReady();
        // TODO: how can we get the key here?
        fprintf(stderr, "%s:%d: Calling KeyUpdate with invalid key (OnKeyReady)!\n", __FILE__,
                __LINE__);
        _callback->KeyUpdate(nullptr, 0);
    } else {
        // TODO: what does this even mean, key ready without key???
        WaitingMessage message = WaitingMessage::ConstructKeyMessage({}, 0);
        _WaitingMessages.push_back(message);
    }
}

void AccessorOCDM::SessionImplementation::Sink::OnKeyError(int16_t f_nError, CDMi::CDMi_RESULT f_crSysError, const char* errorMessage)
{
    TRACE(Trace::Information, ("OnKeyError(%d,%s)", f_nError, errorMessage));
    if (_callback != nullptr) {
        std::string message(errorMessage, strlen(errorMessage));
        //_callback->OnKeyError(f_nError, f_crSysError, message);
        fprintf(stderr, "%s:%d: Calling KeyUpdate with invalid key (OnKeyError)!\n", __FILE__,
                __LINE__);
        _callback->KeyUpdate(nullptr, 0);
    } else {
        // TODO: what does this even mean, key error without key???
        WaitingMessage message = WaitingMessage::ConstructKeyMessage({}, 0);
        _WaitingMessages.push_back(message);
    }
}

//Event fired on key status update
void AccessorOCDM::SessionImplementation::Sink::OnKeyStatusUpdate(const char* keyMessage, const uint8_t buffer[], const uint8_t length)
{
    fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, keyMessage);

    OCDM::ISession::KeyStatus key;

    TRACE(Trace::Information, ("OnKeyStatusUpdate(%s)", keyMessage));

    if (::strcmp(keyMessage, "KeyUsable") == 0)
        key = ::OCDM::ISession::Usable;
    else if (::strcmp(keyMessage, "KeyReleased") == 0)
        key = ::OCDM::ISession::Released;
    else if (::strcmp(keyMessage, "KeyExpired") == 0)
        key = ::OCDM::ISession::Expired;
    else
        key = ::OCDM::ISession::InternalError;

    _parent.UpdateKeyStatus(key, buffer, length);

    if (_callback != nullptr) {
        // TODO: update key state internally in session.
        //_callback->OnKeyStatusUpdate(key);
        fprintf(stderr, "%s:%d %u\n", __FILE__, __LINE__, length);
        _callback->KeyUpdate(buffer, length);
    } else {
        WaitingMessage message = WaitingMessage::ConstructKeyMessage(buffer, length);
        _WaitingMessages.push_back(message);
    }
}

void AccessorOCDM::SessionImplementation::Sink::Callback(OCDM::ISession::ICallback* callback)
{
    if (_callback != nullptr) {
        _callback->Release();
    }

    _callback = callback;

    if (_callback != nullptr) {
        _callback->AddRef();

        for (WaitingMessage & message: _WaitingMessages) {
            switch(message._Type) {
            case WaitingMessage::Challenge:
                _callback->ProcessChallenge(message._Url, &message._Key[0], message._Key.size());
                break;
            case WaitingMessage::Message:
                // TODO: ignore empty keys?
                _callback->KeyUpdate(&message._Key[0], message._Key.size());
                break;
            default:
                ASSERT(!"Unexpected waiting message type.");
                TRACE_L1("ERROR: Unexpected waiting message type: %u!", message._Type);
            }
        }
        _WaitingMessages.clear();
    }
    // TODO: if valid callback, and messages waiting, send messages.
}

AccessorOCDM::SessionImplementation::Sink::WaitingMessage::_WaitingMessage()
    : _Type(MessageType::Invalid)
{
}

AccessorOCDM::SessionImplementation::Sink::WaitingMessage AccessorOCDM::SessionImplementation::Sink::WaitingMessage::ConstructChallenge(const uint8_t key[], uint32_t length, const std::string& url)
{
    WaitingMessage waitingMessage;
    waitingMessage._Type = MessageType::Challenge;

    std::vector<uint8_t> & keyVector = waitingMessage._Key;
    keyVector.resize(length);
    memcpy(&keyVector[0], key, length);

    waitingMessage._Url = url;

    return waitingMessage;
}

AccessorOCDM::SessionImplementation::Sink::WaitingMessage AccessorOCDM::SessionImplementation::Sink::WaitingMessage::ConstructKeyMessage(const uint8_t key[], uint32_t length)
{
    WaitingMessage waitingMessage;
    waitingMessage._Type = MessageType::Message;

    std::vector<uint8_t> & keyVector = waitingMessage._Key;
    keyVector.resize(length);
    memcpy(&keyVector[0], key, length);

    waitingMessage._Url = "";

    return waitingMessage;
}

//AccessorOCDM::SessionImplementation::SessionImplementation(SystemImplementation* parent, CDMi::IMediaKeySession* mediaKeySession, const string& bufferName, const uint32_t defaultSize,
//        const CommonEncryptionData* sessionData) :
//            _parent(*parent),
//            _sink(this, nullptr), // TODO
//            _cencData(*sessionData)
AccessorOCDM::SessionImplementation::SessionImplementation(SystemImplementation* parent, const uint32_t defaultSize, const std::string& bufferName) :
            _parent(*parent),
            _sink(this), // TODO
            //_sink(this, nullptr), // TODO
            //_cencData(*sessionData)
            //_cencData(*CommonEncryptionData::Instance())
            _cencData(nullptr, 0) // TODO
{
    ASSERT(parent != nullptr);
    //ASSERT(sessionData != nullptr);
    //ASSERT(_mediaKeySession != nullptr);

    // TODO: acquire buffer (bufferId)
    // TODO: create MediaKeysSession
    // TODO: default size
    // TODO: isn't CommonEncryptionData a singleton?
    // TODO: we need to register a sink with the OCDMi session

    // TODO: run
    //_mediaKeySession->Run(&_sink);
    //TRACE(Trace::Information, ("Server::Session::Session(%s,%s,%s) => %p", _keySystem.c_str(), _sessionId.c_str(), bufferName.c_str(), this));


    // TODO: so much TODO...
    parent->_mediaKeys.CreateMediaKeySession(0, nullptr, nullptr, 0, nullptr, 0, &_mediaKeySession);

    _buffer = new DataExchange(_mediaKeySession, bufferName, defaultSize);

    _mediaKeySession->Run(&_sink);

    TRACE_L1("Constructed the Session Server side: %p", this);
}

AccessorOCDM::SessionImplementation::~SessionImplementation()
{
    TRACE_L1("Destructing the Session Server side: %p", this);
    // this needs to be done in a thread safe way. Leave it up to
    // the parent to lock handing out new entries before we clear.

    // TODO: Parent should be System, should be notified of our destruction.
    //_parent.Remove(this, _keySystem, _mediaKeySession);

    delete _buffer;

    //TRACE(Trace::Information, ("Server::Session::~Session(%s,%s) => %p", _keySystem.c_str(), _sessionId.c_str(), this));
    TRACE_L1("Destructed the Session Server side: %p", this);
}

//bool AccessorOCDM::SessionImplementation::IsSupported(const CommonEncryptionData& keyIds, const string& keySystem) const
//{
//    return ((keySystem == _keySystem) && (_cencData.IsSupported(keyIds) == true));
//}

bool AccessorOCDM::SessionImplementation::HasKeyId(const uint8_t keyId[])
{
    return (_cencData.HasKeyId(keyId));
    // TODO
    //return false;
}

std::string AccessorOCDM::SessionImplementation::SessionId() const
{
    //return (_sessionId);
    return _mediaKeySession->GetSessionId();
}

OCDM::ISession::KeyStatus AccessorOCDM::SessionImplementation::Status(const uint8_t keyId[],
        const uint8_t length) const
{
    return (_cencData.Status());
}
                
std::string AccessorOCDM::SessionImplementation::BufferId() const
{
    return (_buffer->Name());
}

WPEFramework::Core::Error AccessorOCDM::SessionImplementation::Load()
{
    TRACE(Trace::Information, ("Load()"));
    return CdmiResultToWpeError(_mediaKeySession->Load());
}

// Process a key message response.
void AccessorOCDM::SessionImplementation::Update(const uint8_t* keyMessage,
        const uint16_t keyLength)
{
    TRACE(Trace::Information, ("Update(%d)", keyLength));
    return (_mediaKeySession->Update(keyMessage, keyLength));
}

//Removes all license(s) and key(s) associated with the session
WPEFramework::Core::Error AccessorOCDM::SessionImplementation::Remove()
{
    TRACE(Trace::Information, ("Remove()"));
    return CdmiResultToWpeError(_mediaKeySession->Remove());
}

//We are done with the Session, close what we can..
void AccessorOCDM::SessionImplementation::Close()
{
    TRACE(Trace::Information, ("Close()"));

    _mediaKeySession->Close();
}

uint32_t AccessorOCDM::SessionImplementation::Error(const uint8_t keyId[], const uint8_t length) const
{
    // TODO
    fprintf(stderr, "%s:%d Error\n", __FILE__, __LINE__);
    return 42;
}

WPEFramework::Core::Error AccessorOCDM::SessionImplementation::SystemError() const
{
    // TODO
    return WPEFramework::Core::Error::ERROR_GENERAL;
}

WPEFramework::Core::Error AccessorOCDM::SessionImplementation::Callback(OCDM::ISession::ICallback* callback)
{
    fprintf(stderr, "AccessorOCDM::SessionImplementation::Callback: %p\n", callback);

    _sink.Callback(callback);

    return WPEFramework::Core::Error::ERROR_NONE;
}

WPEFramework::Core::Error AccessorOCDM::SessionImplementation::Decrypt(uint8_t encrypted[], const uint32_t encryptedLength,
        const uint8_t IV[], const uint16_t IVLength)
{
    // TODO
    return WPEFramework::Core::Error::ERROR_GENERAL;
}

void AccessorOCDM::SessionImplementation::UpdateKeyStatus(OCDM::ISession::KeyStatus status, const uint8_t* buffer, const uint8_t length)
{
    // We assume that these UpdateKeyStatusses do not occure in a multithreaded fashion, otherwise we need to lock it.
    CommonEncryptionData::KeyId keyId;
    if (buffer != nullptr) {
        keyId = CommonEncryptionData::KeyId(CommonEncryptionData::COMMON, buffer, length);
    }

    const CommonEncryptionData::KeyId* updated = _cencData.UpdateKeyStatus(status, keyId);

    if (updated != nullptr) {
        const uint8_t length = updated->Length();
        const uint8_t* id = updated->Id();

        TRACE_L1("Reporting a new status for a KeyId. New state: %d", status);

        //_parent.ReportKeyChange(_sessionId, id, length, status);
        // TODO: report this somewhere?
    } else {
        TRACE(Trace::Information, ("There was no key to update !!!"));
    }

}

AccessorOCDM::SystemImplementation::SystemImplementation(AccessorOCDM* parent,
        const std::string keySystem, CDMi::IMediaKeys * mediaKeys) :
        _parent(*parent),
        _refCount(1),
        _keySystem(keySystem),
        _mediaKeys(*mediaKeys)
{
    ASSERT(parent != nullptr);

    // TODO: what to do here?
    // TODO: create _mediaKeySession?

    //_mediaKeySession->Run(&_sink);
    //TRACE(Trace::Information, ("Server::Session::Session(%s,%s,%s) => %p", _keySystem.c_str(), _sessionId.c_str(), bufferName.c_str(), this));
    TRACE_L1("Constructed the System Server side: %p", this);
}

AccessorOCDM::SystemImplementation::~SystemImplementation()
{
    TRACE_L1("Destructing the System Server side: %p", this);
    // this needs to be done in a thread safe way. Leave it up to
    // the parent to lock handing out new entries before we clear.
    //_parent.Remove(this, _keySystem, _mediaKeySession);

    //delete _buffer;

    //TRACE(Trace::Information, ("Server::Session::~Session(%s,%s) => %p", _keySystem.c_str(), _sessionId.c_str(), this));
    //TRACE_L1("Destructed the Session Server side: %p", this);
}

WPEFramework::Core::Error AccessorOCDM::SystemImplementation::SetServerCertificate(
        const uint8_t serverCertificate[], const uint16_t serverCertificateLength)
{
    CDMi::CDMi_RESULT result = _mediaKeys.SetServerCertificate(serverCertificate,
            serverCertificateLength);
    return CdmiResultToWpeError(result);
}

WPEFramework::Core::Error AccessorOCDM::SystemImplementation::CreateSession(
        const int32_t licenseType, const std::string& initDataType, const uint8_t initData[],
        const uint16_t initDataLength, const uint8_t* CDMData, const uint16_t CDMDataLength,
        OCDM::ISession*& session)
{
    std::string bufferId;

    if (!_parent._administrator.AquireBuffer(bufferId)) {
        fprintf(stderr, "%s:%d: ERROR: Failed to get buffer!!\n", __FILE__, __LINE__);
    }

    SessionImplementation * sessionImpl = Core::Service<SessionImplementation>::Create<SessionImplementation>(this, _parent._defaultSize, bufferId);

    session = sessionImpl;

    _parent.Register(sessionImpl);

    return WPEFramework::Core::Error::ERROR_NONE;
}

 
AccessorOCDM::AccessorOCDM(OCDMImplementation* parent, const std::string& name,
        const uint32_t defaultSize) :
            _parent(*parent),
            _adminLock(),
            _administrator(name),
            _defaultSize(defaultSize),
            _sessionList()
{
    ASSERT(parent != nullptr);
}

AccessorOCDM::AccessorOCDM::~AccessorOCDM()
{
    TRACE_L1("Released the AccessorOCDM server side [%d]", __LINE__);
}

WPEFramework::Core::Error AccessorOCDM::AccessorOCDM::IsTypeSupported(const std::string& keySystem,
        const std::string& mimeType) const
{
    if (_parent.IsTypeSupported(keySystem, mimeType)) {
        return WPEFramework::Core::Error::ERROR_NONE;
    } else {
        // TODO: better error code?
        return WPEFramework::Core::Error::ERROR_GENERAL;
    }
}

OCDM::ISystem * AccessorOCDM::AccessorOCDM::CreateSystem(const std::string& keySystem)
{
    CDMi::IMediaKeys* mediaKeys = _parent.KeySystem(keySystem);
    SystemImplementation* newEntry = Core::Service<SystemImplementation>::Create<SystemImplementation>(this, keySystem, mediaKeys);
    return newEntry;
}

OCDM::ISession* AccessorOCDM::AccessorOCDM::Session(const uint8_t data[], const uint8_t keyLength, const uint32_t maxWaitTime)
{
    fprintf(stderr, "%s:%d: Session\n", __FILE__, __LINE__);
    // TODO: use maxWaitTime
    ::OCDM::ISession* result = nullptr;

    if (keyLength >= CommonEncryptionData::KeyId::Length()) {

        fprintf(stderr, "%s:%d: Session\n", __FILE__, __LINE__);
        _adminLock.Lock();

        std::list<SessionImplementation*>::const_iterator index(_sessionList.begin());

        while ((index != _sessionList.end()) && ((*index)->HasKeyId(data) == false)) {
            index++;
        }

        if (index != _sessionList.end()) {
            //printf("Selected session out of list count: %d\n", _sessionList.size());
            result = *index;
            ASSERT(result != nullptr);
            result->AddRef();
        }

        _adminLock.Unlock();
    }

    fprintf(stderr, "%s:%d: Session\n", __FILE__, __LINE__);
    return (result);
}

void AccessorOCDM::Remove(SessionImplementation* session, const string& keySystem,
        CDMi::IMediaKeySession* mediaKeySession)
{
    _adminLock.Lock();

    ASSERT(session != nullptr);

    if (mediaKeySession != nullptr) {

        mediaKeySession->Run(nullptr);

        CDMi::IMediaKeys* system = _parent.KeySystem(keySystem);

        if (system != nullptr) {
            system->DestroyMediaKeySession(mediaKeySession);
        }
    }

    if (session != nullptr) {

        _administrator.ReleaseBuffer(session->BufferId());

        std::list<SessionImplementation*>::iterator index(_sessionList.begin());

        while ((index != _sessionList.end()) && (session != (*index))) {
            index++;
        }

        ASSERT(index != _sessionList.end());

        if (index != _sessionList.end()) {
            const string sessionId(session->SessionId());
            // Before we remove it here, release it.
            _sessionList.erase(index);
            // TODO: do we need to report this somewhere?
            //ReportDestroy(sessionId);
        }
    }

    _adminLock.Unlock();

}

void AccessorOCDM::Register(SessionImplementation * session)
{
    _sessionList.push_back(session);
}

SERVICE_REGISTRATION(OCDMImplementation, 1, 0);

} } /* namespace iWPEFramework::Plugin */
