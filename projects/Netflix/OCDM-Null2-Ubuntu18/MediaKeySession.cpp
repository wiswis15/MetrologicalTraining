#include "MediaKeySession.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

#include <stdio.h>

using namespace std;

namespace CDMi {

struct CallbackInfo
{
	IMediaKeySessionCallback * _callback;
};

static void * UseCallbackTask(void * data)
{
	CallbackInfo * callbackInfo = static_cast<CallbackInfo *>(data);

	sleep(1);

	uint8_t keyMessage[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41 };
	uint32_t keyMessageSize = sizeof(keyMessage);
	char URL[256];	// TODO: why isn't this arg const?
	strcpy(URL, "http://www.a-url.com");

	callbackInfo->_callback->OnKeyMessage(keyMessage, keyMessageSize, URL);

	const char keyMessageStr[] = "KeyUsable";
	callbackInfo->_callback->OnKeyStatusUpdate(keyMessageStr, keyMessage, keyMessageSize);

	delete callbackInfo;

	return nullptr;
}

MediaKeySession::MediaKeySession(const uint8_t drmHeader[], uint32_t drmHeaderLength)
{
	fprintf(stderr, "%s:%d: create media key session ext in null2\n", __FILE__, __LINE__);
}


void MediaKeySession::Run(const IMediaKeySessionCallback *f_piMediaKeySessionCallback)
{
	// TODO: why is this one passed const?
	IMediaKeySessionCallback * nonConstCallback = const_cast<IMediaKeySessionCallback *>(f_piMediaKeySessionCallback);

	CallbackInfo * callbackInfo = new CallbackInfo;
	callbackInfo->_callback = nonConstCallback;

	pthread_t threadId;
	pthread_create(&threadId, nullptr, UseCallbackTask, callbackInfo);
}

CDMi_RESULT MediaKeySession::Load()
{
	cerr << "OCDM-Null2::Load" << endl;
   return 0;
}

void MediaKeySession::Update(const uint8_t *f_pbKeyMessageResponse, uint32_t f_cbKeyMessageResponse)
{
	cerr << "OCDM-Null2::Update, length: " << f_cbKeyMessageResponse << endl;
}

CDMi_RESULT MediaKeySession::Remove()
{
   cerr << "OCDM-Null2::Remove" << endl;
   return 0;
}

CDMi_RESULT MediaKeySession::Close()
{
   cerr << "OCDM-Null2::Close" << endl;
   return 0;
}

const char *MediaKeySession::GetSessionId() const
{
   cerr << "MediaKeySession::GetSessionId: " << this << endl;
   return "ThisIsASessionId";
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
        unsigned long long byteOffset,
        bool initWithLast15)
{
	cerr << "MediaKeySession::Decrypt, buffer size: " << f_cbData << ", IV length: " << f_cbIV << endl;

	// TODO: why const?

	*f_ppbOpaqueClearContent = const_cast<uint8_t *>(f_pbData);
	*f_pcbOpaqueClearContent = f_cbData;
	if (f_cbData > 0) {
		(*f_ppbOpaqueClearContent)[0]++;
	}

   return 0;
}

CDMi_RESULT MediaKeySession::ReleaseClearContent(const uint8_t *f_pbSessionKey, uint32_t f_cbSessionKey, const uint32_t f_cbClearContentOpaque, uint8_t *f_pbClearContentOpaque)
{
   return 0;
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

std::string MediaKeySession::GetContentIdExt() const
{
	return _contentIdExt;
}

void MediaKeySession::SetContentIdExt(const std::string & contentId)
{
	cerr << "Null2 received content id ext: " << contentId << endl;

	_contentIdExt = contentId;
}

LicenseTypeExt MediaKeySession::GetLicenseTypeExt() const
{
	return LimitedDuration;
}

void MediaKeySession::SetLicenseTypeExt(LicenseTypeExt licenseType)
{
}

SessionStateExt MediaKeySession::GetSessionStateExt() const
{
	return mSessionStateExt;
}

void MediaKeySession::SetSessionStateExt(SessionStateExt sessionState)
{
	mSessionStateExt = sessionState;
}

CDMi_RESULT MediaKeySession::SetDrmHeader(const uint8_t drmHeader[], uint32_t drmHeaderLength)
{
	return 0;
}

CDMi_RESULT MediaKeySession::StoreLicenseData(const uint8_t licenseData[], uint32_t licenseDataSize, unsigned char * secureStopId)
{
	secureStopId[15] = 0x42;
	return 0;
}

CDMi_RESULT MediaKeySession::InitDecryptContextByKid()
{
	return 0;
}

CDMi_RESULT MediaKeySession::GetChallengeDataNetflix(uint8_t * challenge, uint32_t & challengeSize, uint32_t isLDL)
{
	return 0;
}

class Null2 : public IMediaKeys, public IMediaKeysExt {
private:
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
    	return 0;
    }

    CDMi_RESULT CommitSecureStop(
            const unsigned char sessionID[],
            uint32_t sessionIDLength,
            const unsigned char serverResponse[],
            uint32_t serverResponseLength) override
    {
    	return 0;
    }

    CDMi_RESULT CreateSystemNetflix(
                const std::string& readDir,
                const std::string& storeLocation) override
	{
    	return 0;
	}

    CDMi_RESULT InitSystemNetflix() override
    {
    	return 0;
    }

    CDMi_RESULT TeardownSystemNetflix() override
    {
    	return 0;
    }

    CDMi_RESULT DeleteSecureStore() override
    {
    	return 0;
    }

    CDMi_RESULT GetSecureStoreHash(
            uint8_t secureStoreHash[],
            uint32_t secureStoreHashLength) override
    {
    	secureStoreHash[secureStoreHashLength - 1] = 0xed;
    	return 0;
    }

};


static SystemFactoryType<Null2> g_instance({"video/x-h264", "audio/mpeg"});

} // namespace CDMi

CDMi::ISystemFactory* GetSystemFactory() {
    return (&CDMi::g_instance);
}

