#ifndef __IOPENCDMI_H
#define __IOPENCDMI_H

#include <core/core.h>

namespace OCDM {

struct ISystem;
struct ISession;

/**
 * \brief Entry point to start using OCDM.
 *
 * This is typically the first class to be instantiated. Use \ref Instance to get the global implementation of this interface.
 */
struct IAccessor : virtual public WPEFramework::Core::IUnknown
{
   /// Interface ID, needed for RPC.
   enum { ID = 0x00000071 };
   virtual ~IAccessor() {}

   /**
    * Returns global instance of \ref IAccessor interface.
    * \return IAccessor singleton.
    */   
   static IAccessor* Instance();

   /**
    * \brief Checks if a DRM system is supported.
    *
    * \param keySystem Name of required key system (e.g. "com.microsoft.playready").
    * \param mimeType MIME type.
    * \return ERROR_NONE if supported, ERROR_UNKNOWN_KEY otherwise.
    * \remark mimeType is currently ignored.
    */
   virtual WPEFramework::Core::Error IsTypeSupported(const std::string& keySystem, const std::string& mimeType) const = 0;

   /**
    * \brief Creates DRM system.
    *
    * \param keySystem Name of required key system (See \ref IsTypeSupported)
    * \return \ref ISystem instance, nullptr on error.
    */
   virtual ISystem * CreateSystem(const std::string& keySystem) = 0;

   /**
    * \brief Maps key ID to \ref ISession instance.
    *
    * In some situations we only have the key ID, but need the specific \ref ISession instance that
    * belongs to this key ID. This method facilitates this requirement.
    * \param keyId Array containing key ID as defined in \ref ISession.
    * \param length Length of keyId array.
    * \param maxWaitTime Maximum allowed time to block (in miliseconds).
    * \return \ref ISession belonging to key ID, or nullptr when not found or timed out.
    */
   virtual ISession* Session(const uint8_t keyId[], const uint8_t length, const uint32_t maxWaitTime) = 0;
};

/**
 * \brief DRM System.
 *
 * Represents an actual DRM system (e.g. PlayReady). 
 */
struct ISystem : virtual public WPEFramework::Core::IUnknown
{
   /// Interface ID, needed for RPC.
   enum { ID = 0x00000074 };
   virtual ~ISystem() {}

   /**
    * \brief Sets server certificate.
    *
    * Some DRMs (e.g. WideVine) use a system-wide server certificate. This method will set that certificate. Other DRMs will ignore this call.
    * \param serverCertificate Buffer containing certificate data.
    * \param serverCertificateLength Buffer length of certificate data.
    * \return Zero on success, non-zero on error.
    */
   virtual WPEFramework::Core::Error SetServerCertificate(const uint8_t serverCertificate[], const uint16_t serverCertificateLength) = 0;

   /**
    * \brief Create DRM session (for actual decrypting of data).
    *
    * Creates an instance of \ref ISession using initialization data.
    * \param licenseType DRM specifc signed integer selecting License Type (e.g. "Limited Duration" for PlayReady).
    * \param initDataType Type of data passed in \ref initData.
    * \param initData Initialization data.
    * \param initDataLength Length (in bytes) of initialization data.
    * \param CDMData CDM data.
    * \param CDMDataLength Length (in bytes) of \ref CDMData.
    * \param session Output parameter that will contain pointer to instance of \ref ISession.
    * \return Zero on success, non-zero on error.
    */
   virtual WPEFramework::Core::Error CreateSession(const int32_t licenseType,
                                     const std::string& initDataType, const uint8_t initData[], const uint16_t initDataLength,
                                     const uint8_t* CDMData, const uint16_t CDMDataLength,
                                     ISession*& session) = 0;
};

/**
 * \brief DRM session (offers data decryption).
 *
 * Is created by \ref ISystem.
 */
struct ISession : virtual public WPEFramework::Core::IUnknown
{
   /// Interface ID, needed for RPC.
   enum { ID = 0x00000072 };
   virtual ~ISession() {}

   /**
    * Reflects status of decryption key.
    */
   enum KeyStatus {
      Usable = 0,       ///< Key is usable.
      Expired,          ///< Key is no longer usable, because too long since licensing.
      Released,         ///< Key was released.
      OutputRestricted, ///< Key's output was restricted.
      OutputDownscaled, ///< When using this key, output will be downscaled.
      StatusPending,    ///< Don't know yet status of key.
      InternalError     ///< Something went wrong.
   };

   /**
    * \brief Callback registered to receive updates about keys.
    */
   struct ICallback : virtual public WPEFramework::Core::IUnknown {
      /// Interface ID, needed for RPC.
      enum { ID = 0x00000073 };
      virtual ~ICallback() {}

      /**
       * Request of process of DRM challenge data. Server is indicated by \ref URL. The response of the server
       * needs to be send to \ref Update.
       *
       * \param URL Target URL to send challenge to.
       * \param keyMessage Buffer containing challenge.
       * \param keyLength Length of challenge (in bytes).
       */
      virtual void ProcessChallenge(const std::string& URL, const uint8_t challenge[], const uint16_t challengeLength) = 0;

      /**
       * Called when status of a key changes.
       *
       * \param keyId Buffer containing key ID.
       * \param length Length of key ID buffer.
       * \param status New status of key.
       */
      virtual void KeyUpdate(const uint8_t keyId[], const uint8_t length) = 0;
   };

   /**
    * Loads the data stored for the specified session into the CDM object.
    * \return Zero on success, non-zero on error.
    */
   virtual WPEFramework::Core::Error Load() = 0;

   /**
    * Process a key message response.
    * \param keyMessage Key message to process.
    * \param keyLength Length of key message buffer (in bytes).
    */
   virtual void Update(const uint8_t keyMessage[], const uint16_t keyLength) = 0;

   /**
    * Removes all keys/licenses related to this session.
    */
   virtual WPEFramework::Core::Error Remove() = 0;
   
   /**
    * Returns current status of this session's key.
    * \return Status of key, see \ref KeyStatus for documentation).
    */
   virtual KeyStatus Status(const uint8_t keyId[], const uint8_t length) const = 0;

   /**
    * Returns error for key (if any).
    *
    * \param keyId Key ID buffer. 
    * \param length Size of buffer (in bytes).
    * \return 
    */
   virtual uint32_t Error(const uint8_t keyId[], const uint8_t length) const = 0;

   /**
    * Returns system error. This reference general system, instead of specific key.
    * \return System error code.
    */
   virtual WPEFramework::Core::Error SystemError() const = 0;

   /**
    * Gets buffer ID for this session.
    * \return Buffer ID.
    */
   virtual std::string BufferId() const = 0;

   /**
    * Returns this session's Session ID (a string).
    * \return Session ID.
    */
   virtual std::string SessionId() const = 0;

   /**
    * Cleans up this Session.
    * \remark Stricly speaking this call isn't needed if this object is correctly dereffed, but there is no risk of "double cleanup"
    *         when using this method. It allows for timely clean-up when timing of the Session's unreffing is hard to predict
    *         (e.g. as a reaction to JavaScript's garbage collector).
    */
   virtual void Close() = 0;

   /**
    * \brief Registers Callback.
    *
    * When a callback is registered after a key status update would have been reported, these updates are still passed to
    * this newly registered callback. During this pass, this class is locked via a Mutex (so no risk of race conditions).
    * Only one callback can be registered at one time. Registering a new callback will unregister the previously registered callback.
    * \param callback Callback to be registered. If callback is a nullptr, the currently registered callback is unregistered.
    * \return Zero on success, non-zero on error.
    */
   virtual WPEFramework::Core::Error Callback(ICallback* callback) = 0;

   /**
    * \brief Performs decryption.
    *
    * This method accepts encrypted data and will typically decrypt it out-of-process (for security reasons). The actual data copying is performed
    * using a memory-mapped file (for performance reasons). If the DRM system allows access to decrypted data (i.e. decrypting is not
    * performed in a TEE), the decryption is performed in-place.
    * \param encrypted Buffer containing encrypted data. If applicable, decrypted data will be stored here after this call returns.
    * \param encryptedLength Length of encrypted data buffer (in bytes).
    * \param IV Initial vector (IV) used during decryption.
    * \param IVLength Length of IV buffer (in bytes).
    * \return Zero on success, non-zero on error.
    */ 
   virtual WPEFramework::Core::Error Decrypt(uint8_t encrypted[], const uint32_t encryptedLength, const uint8_t IV[], const uint16_t IVLength) = 0;
};

}

#endif // __OPENCDMI_

