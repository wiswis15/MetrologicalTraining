#include "open_cdm.h"

#include "AccessorOCDM.h"
#include "IOCDM.h"

#include <cstring>

// TODO: instead returning 0/1, return meaningful values (#defines?)

using namespace WPEFramework;


Core::CriticalSection g_accessorMutex;
OCDM::IAccessor * g_accessor = nullptr;

struct OpenCDMSessionCallbacksCpp : public OCDM::ISession::ICallback
{
    void ProcessChallenge(const std::string& URL, const uint8_t challenge[], const uint16_t challengeLength) override
    {
    	_Callbacks->process_challenge(_UserData, URL.c_str(), challenge, challengeLength);
    }

    void KeyUpdate(const uint8_t keyId[], const uint8_t length) override
    {
    	_Callbacks->key_update(_UserData, keyId, length);
    }

    BEGIN_INTERFACE_MAP(OpenCDMSessionCallbacksCpp)
        INTERFACE_ENTRY(OCDM::ISession::ICallback)
    END_INTERFACE_MAP

    OpenCDMSessionCallbacks * _Callbacks;
    void * _UserData;
};

struct OpenCDMSession
{
	OCDM::ISession * _Session;
	mutable char * _BufferId;
	mutable char * _SessionId;
	OpenCDMSessionCallbacksCpp * _Callbacks;

	OpenCDMSession();
};

OpenCDMSession::OpenCDMSession()
	: _Session(nullptr)
	, _BufferId(nullptr)
	, _SessionId(nullptr)
	, _Callbacks(nullptr)
{
}

static OCDM::IAccessor * GetAccessor()
{
	if (g_accessor == nullptr) {
		g_accessorMutex.Lock();

		if (g_accessor == nullptr) {
			g_accessor = media::AccessorOCDM::Instance();
		}

		g_accessorMutex.Unlock();
	}

	return g_accessor;
}

OpenCDMError opencdm_is_type_supported(const char keySystem[], const char mimeType[])
{
    Core::Error result = GetAccessor()->IsTypeSupported(keySystem, mimeType);

    return (result == Core::ERROR_NONE ? 0 : 1);
}

OpenCDMSystem * opencdm_create_system(const char keySystem[])
{
	OCDM::ISystem * system = GetAccessor()->CreateSystem(keySystem);
	return reinterpret_cast<OpenCDMSystem *>(system);
}

OpenCDMSession * opencdm_get_session(const uint8_t keyId[], uint8_t length, uint32_t maxWaitTime)
{
	OpenCDMSession * output = new OpenCDMSession;
	output->_Session = GetAccessor()->Session(keyId, length, maxWaitTime);
	return output;
}

OpenCDMError opencdm_system_set_server_certificate(OpenCDMSystem * system, const uint8_t serverCertificate[], uint16_t serverCertificateLength)
{
	OCDM::ISystem * ocdmSystem = reinterpret_cast<OCDM::ISystem *>(system);
	WPEFramework::Core::Error result = ocdmSystem->SetServerCertificate(serverCertificate, serverCertificateLength);
	return (result == Core::ERROR_NONE ? 0 : 1);
}

OpenCDMError opencdm_system_create_session(OpenCDMSystem * system, int32_t licenseType,
                                           const char initDataType[], const uint8_t initData[], uint16_t initDataLength,
                                           const uint8_t CDMData[], uint16_t CDMDataLength,
                                           struct OpenCDMSession** session)
{
	OCDM::ISystem * ocdmSystem = reinterpret_cast<OCDM::ISystem *>(system);
	OCDM::ISession * ocdmSession = nullptr;

	(*session) = new OpenCDMSession;
	WPEFramework::Core::Error result = ocdmSystem->CreateSession(licenseType, initDataType, initData, initDataLength ,CDMData, CDMDataLength, (*session)->_Session);
	return (result == Core::ERROR_NONE ? 0 : 1);
}

OpenCDMError opencdm_system_destruct(struct OpenCDMSystem * system)
{
	OCDM::ISystem * ocdmSystem = reinterpret_cast<OCDM::ISystem *>(system);
	ocdmSystem->Release();
	return 0;
}

OpenCDMError opencdm_session_load(OpenCDMSession * session)
{
	WPEFramework::Core::Error result = session->_Session->Load();
	return (result == Core::ERROR_NONE ? 0 : 1);
}

OpenCDMError opencdm_session_update(OpenCDMSession * session, const uint8_t keyMessage[], uint16_t keyLength)
{
	session->_Session->Update(keyMessage, keyLength); // TODO: return value?
	return 0;
}

OpenCDMError opencdm_session_remove(OpenCDMSession * session)
{
	WPEFramework::Core::Error result = session->_Session->Remove();
	return (result == Core::ERROR_NONE ? 0 : 1);
}

KeyStatus opencdm_session_status(const OpenCDMSession * session, const uint8_t keyId[], uint8_t length)
{
	OCDM::ISession::KeyStatus keyStatus = session->_Session->Status(keyId, length);
	return (keyStatus == OCDM::ISession::KeyStatus::Usable ? 0 : 1); // TODO: how best to handle the key status?
}

uint32_t opencdm_session_error(const OpenCDMSession * session, const uint8_t keyId[], uint8_t length)
{
	return session->_Session->Error(keyId, length);
}

OpenCDMError opencdm_session_system_error(const OpenCDMSession * session)
{
	WPEFramework::Core::Error result = session->_Session->SystemError();
	return (result == Core::ERROR_NONE ? 0 : 1);
}

const char * opencdm_session_buffer_id(const OpenCDMSession * session)
{
	if (session->_BufferId == nullptr) {
		std::string bufferId = session->_Session->BufferId();
		session->_BufferId = new char[bufferId.length() + 1];
		std::strcpy(session->_BufferId, bufferId.c_str());
	}

	return session->_BufferId;
}

const char * opencdm_session_session_id(const OpenCDMSession * session)
{
	if (session->_SessionId == nullptr) {
		std::string sessionId = session->_Session->SessionId();
		session->_SessionId = new char[sessionId.length() + 1];
		std::strcpy(session->_SessionId, sessionId.c_str());
	}

	return session->_SessionId;
}

OpenCDMError opencdm_session_close(OpenCDMSession * session)
{
	session->_Session->Close(); // TODO: return value?
	return 0;
}

OpenCDMError opencdm_session_callback(OpenCDMSession * session, OpenCDMSessionCallbacks * callbacks, void * userData)
{
	OpenCDMSessionCallbacksCpp * registeredCallbacks = nullptr;
	if (callbacks != nullptr) {
		ASSERT(callbacks->process_challenge != nullptr);
		ASSERT(callbacks->key_update != nullptr);

		registeredCallbacks = Core::Service<OpenCDMSessionCallbacksCpp>::Create<OpenCDMSessionCallbacksCpp>();
		registeredCallbacks->_Callbacks = callbacks;
		registeredCallbacks->_UserData = userData;
	}

	WPEFramework::Core::Error result = session->_Session->Callback(registeredCallbacks);

	if (session->_Callbacks != nullptr) {
		delete session->_Callbacks;
	}

	session->_Callbacks = registeredCallbacks;

	return (result == Core::ERROR_NONE ? 0 : 1);
}

OpenCDMError opencdm_session_decrypt(OpenCDMSession * session, uint8_t encrypted[], uint32_t encryptedLength, const uint8_t IV[], uint16_t IVLength)
{
	WPEFramework::Core::Error result = session->_Session->Decrypt(encrypted, encryptedLength, IV, IVLength);
	return (result == Core::ERROR_NONE ? 0 : 1);
}

OpenCDMError opencdm_session_destruct(OpenCDMSession * session)
{
	if(session->_BufferId != nullptr) {
		delete [] session->_BufferId;
	}

	if(session->_SessionId != nullptr) {
		delete [] session->_SessionId;
	}

	if(session->_Callbacks != nullptr) {
		delete session->_Callbacks;
	}

	session->_Session->Release();

	delete session;

	return 0;
}





// TODO: destructor?


