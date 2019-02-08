#pragma once

//#include <cdmi.h>
#include <interfaces/IDRM.h>

namespace CDMi {

class MediaKeySession : public IMediaKeySession, public IMediaKeySessionExt
{
public:
    MediaKeySession(const uint8_t drmHeader[], uint32_t drmHeaderLength);

    void Run(const IMediaKeySessionCallback *f_piMediaKeySessionCallback) override;
    CDMi_RESULT Load() override;
    void Update(const uint8_t *f_pbKeyMessageResponse, uint32_t f_cbKeyMessageResponse) override;
    CDMi_RESULT Remove() override;
    CDMi_RESULT Close(void) override;
    const char *GetSessionId(void) const override;
    const char *GetKeySystem(void) const override;
    virtual CDMi_RESULT Decrypt(
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
        bool initWithLast15) override;
    CDMi_RESULT ReleaseClearContent(const uint8_t *f_pbSessionKey, uint32_t f_cbSessionKey, const uint32_t f_cbClearContentOpaque, uint8_t *f_pbClearContentOpaque) override;
    uint32_t GetSessionIdExt(void) const override;

    uint16_t PlaylevelCompressedVideo() const override;
    uint16_t PlaylevelUncompressedVideo() const override;
    uint16_t PlaylevelAnalogVideo() const override;
    uint16_t PlaylevelCompressedAudio() const override;
    uint16_t PlaylevelUncompressedAudio() const override;

    virtual std::string GetContentIdExt() const override;
    virtual void SetContentIdExt(const std::string & contentId) override;
    virtual LicenseTypeExt GetLicenseTypeExt() const override;
    virtual void SetLicenseTypeExt(LicenseTypeExt licenseType) override;
    virtual SessionStateExt GetSessionStateExt() const override;
    virtual void SetSessionStateExt(SessionStateExt sessionState) override;
    virtual CDMi_RESULT SetDrmHeader(const uint8_t drmHeader[], uint32_t drmHeaderLength) override;
    virtual CDMi_RESULT GetChallengeDataNetflix(uint8_t * challenge, uint32_t & challengeSize, uint32_t isLDL) override;
    virtual CDMi_RESULT StoreLicenseData(const uint8_t licenseData[], uint32_t licenseDataSize, unsigned char * secureStopId) override;
    virtual CDMi_RESULT InitDecryptContextByKid() override;

private:
    std::string _contentIdExt;
    SessionStateExt mSessionStateExt;
};

} // namespace CDMi

