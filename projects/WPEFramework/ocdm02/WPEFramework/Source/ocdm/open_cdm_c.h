#ifndef __OCDM_OPEN_CDM_H__
#define __OCDM_OPEN_CDM_H__

// TODO: this header is OK to use (needed for uint32_t)?
#include <inttypes.h>

typedef uint32_t OpenCDMError;
typedef uint32_t KeyStatus;

// TODO: more C-like (typedef)?
// TODO: no need for accessor, right?
struct OpenCDMSystem;
struct OpenCDMSession;

// Callback function pointers definitions
typedef struct OpenCDMSessionCallbacks
{
   void (process_challenge*) (const char url[], const uint8_t challenge[], uint16_t challengeLength, void * userData);
   void (key_update*)        (const uint8_t keyId[], uint8_t length, void * userData);
};


// Function declarations


OpenCDMError opencdm_is_type_supported(const char keySystem[], const char mimeType[]);

OpenCDMSystem * opencdm_create_system(const char keySystem[]);

// TODO: for sure uint8_t length here? Why not uint32_t length for all functions?
OpenCDMSession * opencdm_get_session(const uint8_t keyId[], uint8_t length, uint32_t maxWaitTime);

// OpenCDM session
OpenCDMError opencdm_system_set_server_certificate(OpenCDMSystem * system, const uint8_t serverCertificate[], uint16_t serverCertificateLength);

OpenCDMError opencdm_system_create_session(OpenCDMSystem * system, int32_t licenseType,
                                           const char initDataType[], const uint8_t initData[], uint16_t initDataLength,
                                           const uint8_t CDMData[], uint16_t CDMDataLength,
                                           OpenCDMSession** session);

// OpenCDM system
OpenCDMError opencdm_session_load(OpenCDMSession * session);

// TODO: should this one also return OpenCDMError?
void opencdm_session_update(OpenCDMSession * session, const uint8_t keyMessage[], uint16_t keyLength);

OpenCDMError opencdm_session_remove(OpenCDMSession * session);

KeyStatus opencdm_session_status(const OpenCDMSession * session, const uint8_t keyId[], uint8_t length);

uint32_t opencdm_session_error(const OpenCDMSession * session, const uint8_t keyId[], uint8_t length);

OpenCDMError opencdm_session_system_error(const OpenCDMSession * session);

// TODO: document how to find out required size, and returning error on buffer too small.
OpenCDMError opencdm_session_buffer_id(const OpenCDMSession * session, char * buffer, uint32_t * bufferSize);

OpenCDMError opencdm_session_session_id(const OpenCDMSession * session, char * buffer, uint32_t * bufferSize);

OpenCDMError opencdm_session_close(OpenCDMSession * session);

OpenCDMError opencdm_session_callback(OpenCDMSession * session, OpenCDMSessionCallbacks * callbacks, void * userData);

OpenCDMError opencdm_session_decrypt(OpenCDMSession * session, uint8_t encrypted[], uint32_t encryptedLength, const uint8_t IV[], uint16_t IVLength);

#endif /* __OCDM_OPEN_CDM_H__ */

