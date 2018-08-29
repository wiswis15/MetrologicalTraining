#ifndef __OCDM_OPEN_CDM_H__
#define __OCDM_OPEN_CDM_H__

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * OpenCDM error code. Zero always means success.
 */
typedef uint32_t OpenCDMError;

/**
 * Key status.
 */
typedef uint32_t KeyStatus;

/**
 * Represents a DRM system (e.g. PlayReady).
 */
struct OpenCDMSystem;

/**
 * Represents a OpenCDM session, use this one to decrypt.
 */
struct OpenCDMSession;

/**
 * Registered callbacks with OCDM sessions.
 */
typedef struct
{
   /**
    * Request of process of DRM challenge data. Server is indicated by \ref url. The response of the server
    * needs to be send to \ref opencdm_session_update.
    *
    * \param userData Pointer passed along when \ref opencdm_session_callback was called.
    * \param url Target URL to send challenge to.
    * \param challenge Buffer containing challenge.
    * \param challengeLength Length of challenge (in bytes).
    */
   void (*process_challenge) (void * userData, const char url[], const uint8_t challenge[], uint16_t challengeLength);

   /**
    * Called when status of a key changes. Use \ref opencdm_session_status to find out new key status.
    *
    * \param userData Pointer passed along when \ref opencdm_session_callback was called.
    * \param keyId Buffer containing key ID.
    * \param length Length of key ID buffer.
    */
   void (*key_update)        (void * userData, const uint8_t keyId[], uint8_t length);
} OpenCDMSessionCallbacks;

/**
 * \brief Checks if a DRM system is supported.
 *
 * \param keySystem Name of required key system (e.g. "com.microsoft.playready").
 * \param mimeType MIME type.
 * \return Zero if supported, Non-zero otherwise.
 * \remark mimeType is currently ignored.
 */
OpenCDMError opencdm_is_type_supported(const char keySystem[], const char mimeType[]);

/**
 * \brief Creates DRM system.
 *
 * \param keySystem Name of required key system (See \ref opencdm_is_type_supported)
 * \return \ref OpenCDMSystem instance, NULL on error.
 */
struct OpenCDMSystem * opencdm_create_system(const char keySystem[]);

/**
 * \brief Maps key ID to \ref OpenCDMSession instance.
 *
 * In some situations we only have the key ID, but need the specific \ref OpenCDMSession instance that
 * belongs to this key ID. This method facilitates this requirement.
 * \param keyId Array containing key ID.
 * \param length Length of keyId array.
 * \param maxWaitTime Maximum allowed time to block (in miliseconds).
 * \return \ref OpenCDMSession belonging to key ID, or NULL when not found or timed out.
 */
struct OpenCDMSession * opencdm_get_session(const uint8_t keyId[], uint8_t length, uint32_t maxWaitTime);

/**
 * \brief Sets server certificate.
 *
 * Some DRMs (e.g. WideVine) use a system-wide server certificate. This method will set that certificate. Other DRMs will ignore this call.
 * \param serverCertificate Buffer containing certificate data.
 * \param serverCertificateLength Buffer length of certificate data.
 * \return Zero on success, non-zero on error.
 */
OpenCDMError opencdm_system_set_server_certificate(struct OpenCDMSystem * system, const uint8_t serverCertificate[], uint16_t serverCertificateLength);

/**
 * \brief Create DRM session (for actual decrypting of data).
 *
 * Creates an instance of \ref OpenCDMSession using initialization data.
 * \param licenseType DRM specifc signed integer selecting License Type (e.g. "Limited Duration" for PlayReady).
 * \param initDataType Type of data passed in \ref initData.
 * \param initData Initialization data.
 * \param initDataLength Length (in bytes) of initialization data.
 * \param CDMData CDM data.
 * \param CDMDataLength Length (in bytes) of \ref CDMData.
 * \param session Output parameter that will contain pointer to instance of \ref OpenCDMSession.
 * \return Zero on success, non-zero on error.
 */
OpenCDMError opencdm_system_create_session(struct OpenCDMSystem * system, int32_t licenseType,
                                           const char initDataType[], const uint8_t initData[], uint16_t initDataLength,
                                           const uint8_t CDMData[], uint16_t CDMDataLength,
                                           struct OpenCDMSession** session);

/**
 * Destructs an \ref OpenCDMSystem instance.
 * \param system \ref OpenCDMSystem instance to desctruct.
 * \return Zero on success, non-zero on error.
 */
OpenCDMError opencdm_system_destruct(struct OpenCDMSystem * system);

/**
 * Loads the data stored for a specified OpenCDM session into the CDM context.
 * \param session \ref OpenCDMSession instance.
 * \return Zero on success, non-zero on error.
 */
OpenCDMError opencdm_session_load(struct OpenCDMSession * session);

/**
 * Process a key message response.
 * \param session \ref OpenCDMSession instance.
 * \param keyMessage Key message to process.
 * \param keyLength Length of key message buffer (in bytes).
 * \return Zero on success, non-zero on error.
 */
OpenCDMError opencdm_session_update(struct OpenCDMSession * session, const uint8_t keyMessage[], uint16_t keyLength);

/**
 * Removes all keys/licenses related to a session.
 * \param session \ref OpenCDMSession instance.
 * \return Zero on success, non-zero on error.
 */
OpenCDMError opencdm_session_remove(struct OpenCDMSession * session);

/**
 * Returns status of a particular key assigned to a session.
 * \param session \ref OpenCDMSession instance.
 * \param keyId Key ID.
 * \param length Length of key ID buffer (in bytes).
 * \return key status.
 */
KeyStatus opencdm_session_status(const struct OpenCDMSession * session, const uint8_t keyId[], uint8_t length);

/**
 * Returns error for key (if any).
 * \param session \ref OpenCDMSession instance.
 * \param keyId Key ID.
 * \param length Length of key ID buffer (in bytes).
 * \return Key error (zero if no error, non-zero if error).
 */
uint32_t opencdm_session_error(const struct OpenCDMSession * session, const uint8_t keyId[], uint8_t length);

/**
 * Returns system error. This reference general system, instead of specific key.
 * \param session \ref OpenCDMSession instance.
 * \return System error code, zero if no error.
 */
OpenCDMError opencdm_session_system_error(const struct OpenCDMSession * session);

/**
 * Gets buffer ID for a session.
 * \param session \ref OpenCDMSession instance.
 * \return Buffer ID, valid as long as \ref session is valid.
 */
const char * opencdm_session_buffer_id(const struct OpenCDMSession * session);

/**
 * Gets Session ID for a session.
 * \param session \ref OpenCDMSession instance.
 * \return Session ID, valid as long as \ref session is valid.
 */
const char * opencdm_session_session_id(const struct OpenCDMSession * session);

/**
 * Closes a session.
 * \param session \ref OpenCDMSession instance.
 * \return zero on success, non-zero on error.
 */
OpenCDMError opencdm_session_close(struct OpenCDMSession * session);

/**
 * \brief Registers callbacks with \ref OpenCDMSession instance.
 *
 * Registers callback functions called when a challenge is generated, or a key status changes.
 * If no callbacks were registered but there are key update messages queued, these will be
 * fired to newly registered callbacks. Only one set of callbacks can be registered at one
 * time.
 * \param session \ref OpenCDMSession instance.
 * \param callbacks Callbacks to register, NULL to unregister earlier registered callbacks.
 * \param userData Pointer passed to callbacks when called.
 * \return zero on success, non-zero on error.
 */
OpenCDMError opencdm_session_callback(struct OpenCDMSession * session, OpenCDMSessionCallbacks * callbacks, void * userData);

/**
 * \brief Performs decryption.
 *
 * This method accepts encrypted data and will typically decrypt it out-of-process (for security reasons). The actual data copying is performed
 * using a memory-mapped file (for performance reasons). If the DRM system allows access to decrypted data (i.e. decrypting is not
 * performed in a TEE), the decryption is performed in-place.
 * \param session \ref OpenCDMSession instance.
 * \param encrypted Buffer containing encrypted data. If applicable, decrypted data will be stored here after this call returns.
 * \param encryptedLength Length of encrypted data buffer (in bytes).
 * \param IV Initial vector (IV) used during decryption.
 * \param IVLength Length of IV buffer (in bytes).
 * \return Zero on success, non-zero on error.
 */ 
OpenCDMError opencdm_session_decrypt(struct OpenCDMSession * session, uint8_t encrypted[], uint32_t encryptedLength, const uint8_t IV[], uint16_t IVLength);

/**
 * Destructs an \ref OpenCDMSession instance.
 * \param system \ref OpenCDMSession instance to desctruct.
 * \return Zero on success, non-zero on error.
 */
OpenCDMError opencdm_session_destruct(struct OpenCDMSession * session);

#ifdef __cplusplus
}
#endif

#endif /* __OCDM_OPEN_CDM_H__ */

