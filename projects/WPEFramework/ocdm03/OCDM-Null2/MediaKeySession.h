#pragma once

//#include <cdmi.h>
#include <interfaces/IDRM.h>

namespace CDMi {

class MediaKeySession : public IMediaKeySession
{
public:
    MediaKeySession(const uint8_t *f_pbInitData, uint32_t f_cbInitData);

    void Run(const IMediaKeySessionCallback *f_piMediaKeySessionCallback) override;
    CDMi_RESULT Load() override;
    void Update(const uint8_t *f_pbKeyMessageResponse, uint32_t f_cbKeyMessageResponse) override;
    CDMi_RESULT Remove() override;
    CDMi_RESULT Close(void) override;
    const char *GetSessionId(void) const override;
    const char *GetKeySystem(void) const override;
    //CDMi_RESULT Decrypt(const uint8_t *f_pbSessionKey, uint32_t f_cbSessionKey, const uint32_t *f_pdwSubSampleMapping, uint32_t f_cdwSubSampleMapping, const uint8_t *f_pbIV, uint32_t f_cbIV, const uint8_t *f_pbData, uint32_t f_cbData, uint32_t *f_pcbOpaqueClearContent, uint8_t **f_ppbOpaqueClearContent) override;
    CDMi_RESULT Decrypt(const uint8_t *f_pbSessionKey,
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
        const uint8_t* keyId) override;

    CDMi_RESULT ReleaseClearContent(const uint8_t *f_pbSessionKey, uint32_t f_cbSessionKey, const uint32_t f_cbClearContentOpaque, uint8_t *f_pbClearContentOpaque) override;
};

} // namespace CDMi

