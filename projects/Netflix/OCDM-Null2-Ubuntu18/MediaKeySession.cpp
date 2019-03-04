#include "MediaKeySession.h"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string.h>

#include <stdio.h>

#include <core/core.h>

using namespace std;

namespace CDMi {

struct CallbackInfo
{
	IMediaKeySessionCallback * _callback;
};

static void * KeyStatusUpdateCallback(void * data)
{
	CallbackInfo * callbackInfo = static_cast<CallbackInfo *>(data);

	sleep(1);

	uint8_t keyMessage[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f };
	uint32_t keyMessageSize = sizeof(keyMessage);
	char URL[256];	// TODO: why isn't this arg const?
	strcpy(URL, "http://www.a-url.com");

	callbackInfo->_callback->OnKeyMessage(keyMessage, keyMessageSize, URL);

	const char keyMessageStr[] = "KeyUsable";
	callbackInfo->_callback->OnKeyStatusUpdate(keyMessageStr, keyMessage, keyMessageSize);

	delete callbackInfo;

	return nullptr;
}

static void * PlayLevelUpdateCallback(void * data)
{
	CallbackInfo * callbackInfo = static_cast<CallbackInfo *>(data);
	sleep(1);

    stringstream keyMessage;
    keyMessage << "{";
    keyMessage << "\"compressed-video\": 141,";
    keyMessage << "\"uncompressed-video\": 142,";
    keyMessage << "\"analog-video\": 143,";
    keyMessage << "\"compressed-audio\": 144,";
    keyMessage << "\"uncompressed-audio\": 145";
    keyMessage << "}";

    string keyMessageStr = keyMessage.str();
    const uint8_t * messageBytes = reinterpret_cast<const uint8_t *>(keyMessageStr.c_str());

    callbackInfo->_callback->OnKeyMessage(messageBytes, keyMessageStr.length() + 1, "properties");

	delete callbackInfo;
	return nullptr;
}

MediaKeySession::MediaKeySession(const uint8_t drmHeader[], uint32_t drmHeaderLength)
   : m_piCallback(nullptr)
{
	fprintf(stderr, "%s:%d: create media key session ext in null2\n", __FILE__, __LINE__);

    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    // TODO: should we worry here about multi-threading?
    static int sessionId = 2;
    for (int i = 0; i < (sizeof(m_rgchSessionID) - 1); i++) {
       m_rgchSessionID[i] = (char)sessionId;
    }
    sessionId++;
}

void MediaKeySession::Run(const IMediaKeySessionCallback *f_piMediaKeySessionCallback)
{
	// TODO: why is this one passed const?
	IMediaKeySessionCallback * nonConstCallback = const_cast<IMediaKeySessionCallback *>(f_piMediaKeySessionCallback);
    m_piCallback = nonConstCallback;

	CallbackInfo * callbackInfo = new CallbackInfo;
	callbackInfo->_callback = nonConstCallback;

	pthread_t threadId;
	pthread_create(&threadId, nullptr, KeyStatusUpdateCallback, callbackInfo);
    
	//m_piCallback->OnKeyMessage((const uint8_t *) m_pbChallenge, m_cbChallenge, (char *)m_pchSilentURL);
	//f_piMediaKeySessionCallback->OnKeyMessage((const uint8_t *) m_pbChallenge, m_cbChallenge, (char *)m_pchSilentURL);
	//f_piMediaKeySessionCallback->OnKeyMessage(NULL, 0, NULL);
	//nonConstCallback->OnKeyMessage(NULL, 0, NULL);
	uint8_t challengeBuffer[32];
	uint32_t challengeBufferSize = sizeof(challengeBuffer);
    char urlBuffer[32];
    urlBuffer[0] = '\0';
	nonConstCallback->OnKeyMessage(challengeBuffer, challengeBufferSize, urlBuffer);
}

CDMi_RESULT MediaKeySession::Load()
{
	cerr << "OCDM-Null2::Load" << endl;
   return (CDMi_RESULT)0;
}

void MediaKeySession::Update(const uint8_t *f_pbKeyMessageResponse, uint32_t f_cbKeyMessageResponse)
{
	cerr << "OCDM-Null2::Update, length: " << f_cbKeyMessageResponse << endl;
}

CDMi_RESULT MediaKeySession::Remove()
{
   cerr << "OCDM-Null2::Remove" << endl;
   return (CDMi_RESULT)0;
}

CDMi_RESULT MediaKeySession::Close()
{
   cerr << "OCDM-Null2::Close" << endl;
   return (CDMi_RESULT)0;
}

const char *MediaKeySession::GetSessionId() const
{
   cerr << "MediaKeySession::GetSessionId: " << this << endl;
   //return "ThisIsASessionId";
   return m_rgchSessionID;
}

const char *MediaKeySession::GetKeySystem() const
{
   return "Null2";
}

CDMi_RESULT MediaKeySession::Decrypt(
        const uint8_t *f_pbSessionKey,
        uint32_t f_cbSessionKey,
        const uint32_t *f_pdwSubSampleMapping,
        uint32_t f_cdwSubSampleMapping,
        const uint8_t *f_pbIV,
        uint32_t f_cbIV,
        const uint8_t *f_pbData,
        uint32_t f_cbData,
        uint32_t *f_pcbOpaqueClearContent,
        uint8_t **f_ppbOpaqueClearContent,
        const uint8_t keyIdLength,
        const uint8_t* keyId,
        bool initWithLast15)
{
	cerr << "MediaKeySession::Decrypt, buffer size: " << f_cbData << ", IV length: " << f_cbIV << endl;

	// TODO: why const?

	*f_ppbOpaqueClearContent = const_cast<uint8_t *>(f_pbData);
	*f_pcbOpaqueClearContent = f_cbData;
	if (f_cbData > 0) {
		(*f_ppbOpaqueClearContent)[0]++;
	}

   return (CDMi_RESULT)0;
}

CDMi_RESULT MediaKeySession::ReleaseClearContent(const uint8_t *f_pbSessionKey, uint32_t f_cbSessionKey, const uint32_t f_cbClearContentOpaque, uint8_t *f_pbClearContentOpaque)
{
   return (CDMi_RESULT)0;
}

uint32_t MediaKeySession::GetSessionIdExt() const
{
	cerr << "Null2 session is asked for Session ID Ext" << endl;
	return 56;
}

uint16_t MediaKeySession::PlaylevelCompressedVideo() const
{
	cerr << "Null2 session is asked for PlaylevelCompressedVideo" << endl;
	return 57;
}

uint16_t MediaKeySession::PlaylevelUncompressedVideo() const
{
	return 58;
}

uint16_t MediaKeySession::PlaylevelAnalogVideo() const
{
	return 59;
}

uint16_t MediaKeySession::PlaylevelCompressedAudio() const
{
	return 60;
}

uint16_t MediaKeySession::PlaylevelUncompressedAudio() const
{
	return 61;
}

CDMi_RESULT MediaKeySession::SetDrmHeader(const uint8_t drmHeader[], uint32_t drmHeaderLength)
{
	return (CDMi_RESULT)0;
}

CDMi_RESULT MediaKeySession::StoreLicenseData(const uint8_t licenseData[], uint32_t licenseDataSize, unsigned char * secureStopId)
{
	secureStopId[15] = 0x42;
	return (CDMi_RESULT)0;
}

CDMi_RESULT MediaKeySession::InitDecryptContextByKid()
{
	CallbackInfo * callbackInfo = new CallbackInfo;
	callbackInfo->_callback = m_piCallback;

	pthread_t threadId;
	pthread_create(&threadId, nullptr, PlayLevelUpdateCallback, callbackInfo);
	return (CDMi_RESULT)0;
}

CDMi_RESULT MediaKeySession::GetChallengeDataNetflix(uint8_t * challenge, uint32_t & challengeSize, uint32_t isLDL)
{
	return (CDMi_RESULT)0;
}

class Null2 : public IMediaKeys, public IMediaKeysExt {
private:
    class Config : public WPEFramework::Core::JSON::Container {
    private:
        Config& operator= (const Config&);

    public:
        Config () 
            : ReadDir()
            , StoreLocation() {
            Add("read-dir", &ReadDir);
            Add("store-location", &StoreLocation);
        }
        Config (const Config& copy) 
            : ReadDir(copy.ReadDir)
            , StoreLocation(copy.StoreLocation) {
            Add("read-dir", &ReadDir);
            Add("store-location", &StoreLocation);
        }
        virtual ~Config() {
        }

    public:
        WPEFramework::Core::JSON::String ReadDir;
        WPEFramework::Core::JSON::String StoreLocation;
    };

    Null2 (const Null2&) = delete;
    Null2& operator= (const Null2&) = delete;

public:
    Null2() {
    }

    ~Null2(void) {
    }

    CDMi_RESULT CreateMediaKeySession(
        int32_t licenseType,
        const char *f_pwszInitDataType,
        const uint8_t *f_pbInitData,
        uint32_t f_cbInitData, 
        const uint8_t *f_pbCDMData,
        uint32_t f_cbCDMData, 
        IMediaKeySession **f_ppiMediaKeySession) {

        *f_ppiMediaKeySession = new CDMi::MediaKeySession(f_pbInitData, f_cbInitData);
 
        return CDMi_SUCCESS; 
    }

    virtual CDMi_RESULT SetServerCertificate(
            const uint8_t *f_pbServerCertificate,
            uint32_t f_cbServerCertificate) override
    {
    	cerr << "OCDM-Null2::SetServerCertificate" << endl;

        return CDMi_SUCCESS;
    }

    CDMi_RESULT DestroyMediaKeySession(IMediaKeySession *f_piMediaKeySession) {

        delete f_piMediaKeySession;

        return CDMi_SUCCESS; 
    }

    time_t GetDrmSystemTime() const override
    {
       fprintf(stderr, "%s:%d: Null2 is asked for system time\n", __FILE__, __LINE__);
       return 46;
    }

    CDMi_RESULT CreateMediaKeySessionExt(
            const uint8_t drmHeader[],
            uint32_t drmHeaderLength,
            IMediaKeySessionExt** session) override
	{

        *session = new CDMi::MediaKeySession(drmHeader, drmHeaderLength);

        fprintf(stderr, "%s:%d: Null2 created a session, drmHeaderLength: %u\n", __FILE__, __LINE__, drmHeaderLength);

        return CDMi_SUCCESS;
	}

    std::string GetVersionExt() const override
    {
    	return "Null2-version";
    }

    uint32_t GetLdlSessionLimit() const override
    {
    	return 17;
    }

    CDMi_RESULT EnableSecureStop(bool enable) override
    {
    	return (CDMi_RESULT)0;
    }

    CDMi_RESULT CommitSecureStop(
            const unsigned char sessionID[],
            uint32_t sessionIDLength,
            const unsigned char serverResponse[],
            uint32_t serverResponseLength) override
    {
    	return (CDMi_RESULT)0;
    }

    CDMi_RESULT CreateSystemNetflix() override
	{
      cerr << "CreateSystemNetflix" << endl;
      cerr << "readir: " << m_readDir << endl;
      cerr << "store: " << m_storeLocation << endl;

    	return (CDMi_RESULT)0;
	}

    CDMi_RESULT InitSystemNetflix() override
    {
    	return (CDMi_RESULT)0;
    }

    CDMi_RESULT TeardownSystemNetflix() override
    {
    	return (CDMi_RESULT)0;
    }

    CDMi_RESULT DeleteSecureStore() override
    {
    	return (CDMi_RESULT)0;
    }

    CDMi_RESULT GetSecureStoreHash(
            uint8_t secureStoreHash[],
            uint32_t secureStoreHashLength) override
    {
    	secureStoreHash[secureStoreHashLength - 1] = 0xed;
    	return (CDMi_RESULT)0;
    }


    void OnSystemConfigurationAvailable(const std::string& configline)
    {
       //cerr << "OnSystemConfigurationAvailable: " << configline << endl;

        Config config; 
        config.FromString(configline);
        m_readDir = config.ReadDir.Value();
        m_storeLocation = config.StoreLocation.Value();

        //cerr << "readir: " << m_readDir << endl;
        //cerr << "store: " << m_storeLocation << endl;
    }

    virtual CDMi_RESULT DestroyMediaKeySessionExt(IMediaKeySession*) override
    {
        return (CDMi_RESULT)0;
    }

    virtual bool IsSecureStopEnabled() override
    {
        return true;
    }

    virtual uint32_t ResetSecureStops() override
    {
        return 0;
    }

    virtual CDMi_RESULT GetSecureStopIds(uint8_t**, uint32_t&) override
    {
        return (CDMi_RESULT)0;
    }

    virtual CDMi_RESULT GetSecureStop(const uint8_t*, uint32_t, uint8_t*, uint16_t&) override
    {
        return (CDMi_RESULT)0;
    }

    virtual CDMi_RESULT DeleteKeyStore() override
    {
        return (CDMi_RESULT)0;
    }

    virtual CDMi_RESULT GetKeyStoreHash(uint8_t*, uint32_t) override
    {
        return (CDMi_RESULT)0;
    }

private:
   string m_readDir;
   string m_storeLocation;
};


static SystemFactoryType<Null2> g_instance({"video/x-h264", "audio/mpeg"});

} // namespace CDMi

CDMi::ISystemFactory* GetSystemFactory() {
    return (&CDMi::g_instance);
}

