#pragma once

#include "Module.h"

// Get in the definitions required for access to the sepcific
// DRM engines.
#include <interfaces/IDRM.h>

// Get in the definitions required for access to the OCDM
// counter part living in the applications
#include <ocdm/IOCDM.h>
#include <ocdm/DataExchange.h>

#include <interfaces/IContentDecryption.h>
#include <vector>
#include <cstring>

#include "CENCParser.h"

namespace WPEFramework {
namespace Plugin {

class OCDMImplementation;

class AccessorOCDM: public OCDM::IAccessor {
private:
    class SystemImplementation;
    class SessionImplementation;

    struct SessionKey {
        // TODO: do we need this?
        SessionKey() {
        }

        SessionKey(const uint8_t buffer[], uint32_t length) {
            _Data.resize(length);
            std::memcpy(&_Data[0], buffer, length);
        }

        bool operator==(const SessionKey& rhs) const {
            return (_Data == rhs._Data);
        }

        bool operator<(const SessionKey& rhs) const {
            return (_Data < rhs._Data);
        }

        std::vector<uint8_t> _Data;
    };

private:
    class BufferAdministrator {
    private:
        BufferAdministrator() = delete;
        BufferAdministrator(const BufferAdministrator&) = delete;
        BufferAdministrator& operator=(const BufferAdministrator&) = delete;

    public:
        BufferAdministrator(const string& pathName);
        ~BufferAdministrator();

    public:
        bool AquireBuffer(string& locator);
        bool ReleaseBuffer(const string& locator);
    private:
        Core::CriticalSection _adminLock;
        string _basePath;
        uint16_t _occupation;
    };

public:
    AccessorOCDM(OCDMImplementation* parent, const string& name, const uint32_t defaultSize);
    virtual ~AccessorOCDM();

    virtual WPEFramework::Core::Error IsTypeSupported(const std::string& keySystem, const std::string& mimeType) const override;
    virtual OCDM::ISystem * CreateSystem(const std::string& keySystem) override;
    virtual ::OCDM::ISession* Session(const uint8_t data[], const uint8_t keyLength, const uint32_t maxWaitTime) override;

    BEGIN_INTERFACE_MAP(AccessorOCDM)
        INTERFACE_ENTRY(::OCDM::IAccessor)
    END_INTERFACE_MAP

private:
    OCDMImplementation& _parent;
    mutable Core::CriticalSection _adminLock;
    BufferAdministrator _administrator;
    uint32_t _defaultSize;
    std::list<SessionImplementation*> _sessionList;

private:
    //OCDM::ISession* FindSession (const CommonEncryptionData& keyIds, const string& keySystem) const;
    void Remove(SessionImplementation* session, const string& keySystem, CDMi::IMediaKeySession* mediaKeySession);
    void Register(SessionImplementation * session);

private:
    AccessorOCDM() = delete;
    AccessorOCDM(const AccessorOCDM&) = delete;
    AccessorOCDM& operator=(const AccessorOCDM&) = delete;

    class SystemImplementation : public OCDM::ISystem {
    private:
        SystemImplementation() = delete;
        SystemImplementation(const SystemImplementation&) = delete;
        SystemImplementation& operator= (const SystemImplementation&) = delete;

    public:
        SystemImplementation(AccessorOCDM* parent, const std::string keySystem, CDMi::IMediaKeys * mediaKeys);
        virtual ~SystemImplementation();

        WPEFramework::Core::Error SetServerCertificate(const uint8_t serverCertificate[], const uint16_t serverCertificateLength) override;
        WPEFramework::Core::Error CreateSession(const int32_t licenseType, const std::string& initDataType, const uint8_t initData[], const uint16_t initDataLength, const uint8_t* CDMData,
                const uint16_t CDMDataLength, OCDM::ISession*& session) override;

        BEGIN_INTERFACE_MAP(SystemImplementation)
            INTERFACE_ENTRY(OCDM::ISystem)
        END_INTERFACE_MAP

    private:
        AccessorOCDM & _parent;
        mutable uint32_t _refCount;
        std::string _keySystem;
        std::vector<SessionImplementation*> _sessions;
        CDMi::IMediaKeys & _mediaKeys;

        friend SessionImplementation;
    };

    class SessionImplementation : public ::OCDM::ISession {
    private:
        SessionImplementation() = delete;
        SessionImplementation(const SessionImplementation&) = delete;
        SessionImplementation& operator= (const SessionImplementation&) = delete;

    private:
        class DataExchange: public ::OCDM::DataExchange, public Core::Thread {
        private:
            DataExchange() = delete;
            DataExchange(const DataExchange&) = delete;
            DataExchange& operator= (const DataExchange&) = delete;

        public:
            DataExchange(CDMi::IMediaKeySession* mediaKeys, const string& name, const uint32_t defaultSize);
            ~DataExchange();

        private:
            virtual uint32_t Worker () override;
        private:
            CDMi::IMediaKeySession* _mediaKeys;
            uint8_t* _sessionKey;
            uint32_t _sessionKeyLength;
        };

        class Sink: public CDMi::IMediaKeySessionCallback {
        private:
            Sink() = delete;
            Sink(const Sink&) = delete;
            Sink& operator= (const Sink&) = delete;

        public:
            Sink(SessionImplementation* parent);
            virtual ~Sink();

        public:
            // Event fired when a key message is successfully created.
            virtual void OnKeyMessage(const uint8_t *f_pbKeyMessage, uint32_t f_cbKeyMessage, char *f_pszUrl) override;

            // Event fired when MediaKeySession has found a usable key.
            virtual void OnKeyReady() override;

            // Event fired when MediaKeySession encounters an error.
            virtual void OnKeyError( int16_t f_nError, CDMi::CDMi_RESULT f_crSysError, const char* errorMessage) override;

            //Event fired on key status update
            virtual void OnKeyStatusUpdate(const char* keyMessage, const uint8_t buffer[], const uint8_t length) override;

            void Callback(OCDM::ISession::ICallback* callback);

            OCDM::ISession::KeyStatus Status(const uint8_t keyId[], const uint8_t length) const;

            uint32_t Error(const uint8_t keyId[], const uint8_t length) const;

       private:
            SessionImplementation& _parent;
            OCDM::ISession::ICallback* _callback;

            // TODO: is the underscore worth the shorter names?
            typedef struct _WaitingMessage
            {
                typedef enum {
                    Invalid,
                    Challenge,
                    Message
                } MessageType;

                MessageType _Type;
                SessionKey _Key;
                std::string _Url;

                static _WaitingMessage ConstructChallenge(const uint8_t key[], uint32_t length, const std::string& url);
                static _WaitingMessage ConstructKeyMessage(const uint8_t key[], uint32_t length);

            private:
                _WaitingMessage();
            } WaitingMessage;

            std::vector<WaitingMessage> _WaitingMessages;

            std::map<SessionKey, OCDM::ISession::KeyStatus> _KeyStatusMap;
            std::map<SessionKey, uint32_t> _KeyErrorMap;
        };

    public:
        SessionImplementation(SystemImplementation* parent, const uint32_t defaultSize, const std::string& bufferName);

        virtual ~SessionImplementation();

    public:
        //inline bool IsSupported (const CommonEncryptionData& keyIds, const string& keySystem) const;
        bool HasKeyId(const uint8_t keyId[]);
        virtual std::string SessionId() const override;

        virtual OCDM::ISession::KeyStatus Status(const uint8_t keyId[], const uint8_t length) const override;

        virtual std::string BufferId() const override;

        virtual WPEFramework::Core::Error Load() override;

        virtual void Update(const uint8_t* keyMessage, const uint16_t keyLength) override;

        virtual WPEFramework::Core::Error Remove() override;

        virtual void Close() override;

        virtual uint32_t Error(const uint8_t keyId[], const uint8_t length) const override;

        virtual WPEFramework::Core::Error SystemError() const override;

        virtual WPEFramework::Core::Error Callback(ICallback* callback) override;

        virtual WPEFramework::Core::Error Decrypt(uint8_t encrypted[], const uint32_t encryptedLength, const uint8_t IV[], const uint16_t IVLength) override;

        BEGIN_INTERFACE_MAP(SessionImplementation)
            INTERFACE_ENTRY(OCDM::ISession)
        END_INTERFACE_MAP

    private:
        void UpdateKeyStatus(::OCDM::ISession::KeyStatus status, const uint8_t* buffer, const uint8_t length);

    private:
        SystemImplementation& _parent;
        mutable Core::CriticalSection _adminLock;
        //mutable uint32_t _refCount;
        std::string _sessionId;
        CDMi::IMediaKeySession* _mediaKeySession;
        //Core::Sink<Sink> _sink;
        //Core::Sink<Sink> * _sink; // TODO
        Sink _sink;
        DataExchange* _buffer;
        //CommonEncryptionData & _cencData; // TODO: singleton?
        CommonEncryptionData _cencData;
    };
};

}
}
