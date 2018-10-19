#include "MediaKeySession.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

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

MediaKeySession::MediaKeySession(const uint8_t *f_pbInitData, uint32_t f_cbInitData)
{
}

MediaKeySession::MediaKeySession(uint32_t sessionId, const char contentId[], uint32_t contentIdLength, LicenseTypeExt licenseType, const uint8_t drmHeader[], uint32_t drmHeaderLength)
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

CDMi_RESULT MediaKeySession::Decrypt(const uint8_t *f_pbSessionKey, uint32_t f_cbSessionKey, const uint32_t *f_pdwSubSampleMapping,
		uint32_t f_cdwSubSampleMapping, const uint8_t *f_pbIV, uint32_t f_cbIV, const uint8_t *f_pbData,
		uint32_t f_cbData, uint32_t *f_pcbOpaqueClearContent, uint8_t **f_ppbOpaqueClearContent, const uint8_t keyIdLength, const uint8_t* keyId)
{
	cerr << "MediaKeySession::Decrypt, buffer size: " << f_cbData << ", IV length: " << f_cbIV << endl;

	// TODO: why const?

	*f_ppbOpaqueClearContent = const_cast<uint8_t *>(f_pbData);
	*f_pcbOpaqueClearContent = f_cbData;
	if (f_cbData > 0) {
		(*f_ppbOpaqueClearContent)[0] = 0x80;
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

    CDMi_RESULT CreateMediaKeySessionExt(uint32_t sessionId,
            const char contentId[],
            uint32_t contentIdLength,
            LicenseTypeExt licenseType,
            const uint8_t drmHeader[],
            uint32_t drmHeaderLength,
            IMediaKeySessionExt** session) override
	{

        *session = new CDMi::MediaKeySession(sessionId, contentId, contentIdLength, licenseType, drmHeader, drmHeaderLength);

        fprintf(stderr, "%s:%d: Null2 created a session\n", __FILE__, __LINE__);

        return CDMi_SUCCESS;
	}

};


static SystemFactoryType<Null2> g_instance({"video/x-h264", "audio/mpeg"});

} // namespace CDMi

CDMi::ISystemFactory* GetSystemFactory() {
    return (&CDMi::g_instance);
}

