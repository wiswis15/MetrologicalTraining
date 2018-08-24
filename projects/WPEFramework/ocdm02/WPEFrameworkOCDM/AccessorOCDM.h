#pragma once

#include <opencdm/IOCDM.h>
#include <core/core.h>
#include <com/com.h>

// TODO: better not to use namespace in header
using namespace WPEFramework;

namespace media {

extern Core::CriticalSection _systemLock;


// TODO: fully move to cpp file.

class AccessorOCDM : public OCDM::IAccessor {
private:
    AccessorOCDM () = delete;
    AccessorOCDM (const AccessorOCDM&) = delete;
    AccessorOCDM& operator= (const AccessorOCDM&) = delete;

private:
    class RPCClient {
    private:
        RPCClient() = delete;
        RPCClient(const RPCClient&) = delete;
        RPCClient& operator=(const RPCClient&) = delete;

        typedef WPEFramework::RPC::InvokeServerType<4, 1> RPCService;

    public:
        RPCClient(const Core::NodeId& nodeId)
            : _client(Core::ProxyType<RPC::CommunicatorClient>::Create(nodeId))
            , _service(Core::ProxyType<RPCService>::Create(Core::Thread::DefaultStackSize())) {

            _client->CreateFactory<RPC::InvokeMessage>(2);
            if (_client->Open(RPC::CommunicationTimeOut) == Core::ERROR_NONE) {

                // TODO, Seems the announce is still progressing, make sure the open blocks, till it completes.
                SleepMs(100);

                // I would not expect that the next line is needed, left it in for reference for testing.
                // If it is neede, it needs to move to the RPC::CommunicatorClient..
                _client->Register(_service);
            }
            else {
                _client.Release();
            }
        }
        ~RPCClient() {
            if (_client.IsValid() == true) {
                _client->Unregister(_service);
                _client.Release();
            }
        }

    public:
        inline bool IsOperational() const {
            return (_client.IsValid());
        }

        template <typename INTERFACE>
        INTERFACE* Create(const string& objectName, const uint32_t version = static_cast<uint32_t>(~0)) {
            INTERFACE* result = nullptr;
            
            fprintf(stderr, "SANDER %s:%d: Creating \"%s\"\n", __FILE__, __LINE__, objectName.c_str());

            if (_client.IsValid() == true) {
                // Oke we could open the channel, lets get the interface
                result = _client->Create<INTERFACE>(objectName, version);
                fprintf(stderr, "SANDER %s:%d: Created \"%s\": %p\n", __FILE__, __LINE__, objectName.c_str(), result);
            }

            return (result);
        }

    private:
        Core::ProxyType<RPC::CommunicatorClient> _client;
        Core::ProxyType<RPCService> _service;
    };

private:
    AccessorOCDM (const TCHAR domainName[]) 
        : _refCount(1)
        , _client(Core::NodeId(domainName))
        , _remote(nullptr) {

        if (_client.IsOperational() == true) { 
            _remote = _client.Create<OCDM::IAccessor>(_T(""));
        }
    }

public:

    static OCDM::IAccessor* Instance();

/*
    static OCDM::IAccessor* Instance () {

        _systemLock.Lock();

        if (_singleton == nullptr) {
            AccessorOCDM* result = new AccessorOCDM ("/tmp/ocdm"); 

            if (result->_remote != nullptr) {
                _singleton = result;
            }
        } 
        else {
            _singleton->AddRef();
        }

        _systemLock.Unlock();

        return (_singleton);
    }
*/

    ~AccessorOCDM() {
        if (_remote != nullptr) {
            _remote->Release();
        }
        _singleton = nullptr;
        TRACE_L1("Destructed the AccessorOCDM %p", this);
    }

private:
    virtual void AddRef() const override {
        Core::InterlockedIncrement(_refCount);
    }

    virtual uint32_t Release() const override {

        _systemLock.Lock();

        if (Core::InterlockedDecrement(_refCount) == 0) {
            delete this;
            return (Core::ERROR_DESTRUCTION_SUCCEEDED);
        }

        _systemLock.Unlock();

        return (Core::ERROR_NONE);
    }
    
    BEGIN_INTERFACE_MAP(AccessorOCDM)
        INTERFACE_ENTRY(OCDM::IAccessor)
    END_INTERFACE_MAP
    
    virtual WPEFramework::Core::Error IsTypeSupported(const std::string& keySystem, const std::string& mimeType) const override;

/*
    virtual Core::Error IsTypeSupported(
        const std::string& keySystem,
        const std::string& mimeType) const override {
        return _remote->IsTypeSupported(keySystem, mimeType);
    }
*/

    virtual OCDM::ISystem * CreateSystem(const std::string& keySystem) override {
       return _remote->CreateSystem(keySystem);
    }

    virtual OCDM::ISession* Session(const uint8_t keyId[], const uint8_t length, const uint32_t maxWaitTime) override {
        return (_remote->Session(keyId, length, maxWaitTime));
    }

private:
    mutable uint32_t _refCount;
    RPCClient _client;
    OCDM::IAccessor* _remote;
    static AccessorOCDM* _singleton;

};

}

