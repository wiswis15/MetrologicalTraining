#include <opencdm/open_cdm.h>
#include <opencdm/open_cdm_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include <core/core.h>

using namespace WPEFramework::Core;

class PlayLevels : public JSON::Container {
private:
    PlayLevels(const PlayLevels&) = delete;
    PlayLevels& operator=(const PlayLevels&) = delete;

public:
   PlayLevels()
      : WPEFramework::Core::JSON::Container()
      , m_CompressedVideo(0)
      , m_UncompressedVideo(0)
      , m_AnalogVideo(0)
      , m_CompressedAudio(0)
      , m_UncompressedAudio(0)
   {
      Add(_T("compressed-video"), &m_CompressedVideo);
      Add(_T("uncompressed-video"), &m_UncompressedVideo);
      Add(_T("analog-video"), &m_AnalogVideo);
      Add(_T("compressed-audio"), &m_CompressedAudio);
      Add(_T("uncompressed-audio"), &m_UncompressedAudio);
   }

public:
   JSON::DecUInt16 m_CompressedVideo;
   JSON::DecUInt16 m_UncompressedVideo;
   JSON::DecUInt16 m_AnalogVideo;
   JSON::DecUInt16 m_CompressedAudio;
   JSON::DecUInt16 m_UncompressedAudio;
};

void MyProcessChallenge(struct OpenCDMSession* session, const char url[], const uint8_t challenge[], const uint16_t challengeLength)
{
   fprintf(stderr, "Callback: %s (%s:%d) -- %s\n", __FUNCTION__, __FILE__, __LINE__, challenge);

   if (strcmp(url, "properties") == 0) {
      std::string challengeString(reinterpret_cast<const char *>(challenge), challengeLength);
      PlayLevels playLevels;
      playLevels.FromString(challengeString);

      fprintf(stderr, " - Compressed video: %u\n", playLevels.m_CompressedVideo.Value());
      fprintf(stderr, " - Uncompressed video: %u\n", playLevels.m_UncompressedVideo.Value());
      fprintf(stderr, " - Analog video: %u\n", playLevels.m_AnalogVideo.Value());
      fprintf(stderr, " - Compressed audio: %u\n", playLevels.m_CompressedAudio.Value());
      fprintf(stderr, " - Uncompressed audio: %u\n", playLevels.m_UncompressedAudio.Value());
   }
}

void MyKeyUpdate(struct OpenCDMSession* session, const uint8_t keyId[], const uint8_t length)
{
   fprintf(stderr, "Callback: %s (%s:%d) -- %s\n", __FUNCTION__, __FILE__, __LINE__, keyId);
}

void MyCallbackMessage(struct OpenCDMSession* userData, const char message[])
{
   fprintf(stderr, "Callback: %s (%s:%d), message: %s\n", __FUNCTION__, __FILE__, __LINE__, message);
}

OpenCDMSessionCallbacks g_callbacks = {
    .process_challenge = MyProcessChallenge,
    .key_update = MyKeyUpdate,
    .message = MyCallbackMessage
};

int main()
{
   OpenCDMAccessor * accessor = opencdm_create_system();

   //const char keySystem[] = "com.metrological.null";
   const char keySystem[] = "com.netflix.playready";
   OpenCDMSystemExt* systemExt = opencdm_create_system_ext(accessor, keySystem);
   fprintf(stderr, "Created system ext: %p\n", systemExt);

   OpenCDMError isSupported = opencdm_is_type_supported(accessor, keySystem, "");
   fprintf(stderr, "%s is supported (should be 0): %u\n", keySystem, isSupported);

   uint64_t drmSystemTime;

   fprintf(stderr, "About to call opencdm_system_get_drm_time\n");
   opencdm_system_get_drm_time(accessor, keySystem, &drmSystemTime);
   fprintf(stderr, "Called opencdm_system_get_drm_time: %" PRIu64 "\n", drmSystemTime);

   char versionStr01[65];
   fprintf(stderr, "About to call opencdm_system_get_version\n");
   opencdm_system_get_version(accessor, keySystem, versionStr01);
   fprintf(stderr, "Called opencdm_system_get_version: %s\n", versionStr01);

   uint32_t ldlLimit01;
   fprintf(stderr, "About to call opencdm_system_get_ldl_session_limit\n");
   opencdm_system_ext_get_ldl_session_limit(systemExt, &ldlLimit01);
   fprintf(stderr, "Called opencdm_system_get_ldl_session_limit: %u\n", ldlLimit01);

   fprintf(stderr, "About to call opencdm_system_enable_secure_stop\n");
   opencdm_system_ext_enable_secure_stop(systemExt, 1);
   fprintf(stderr, "Called opencdm_system_enable_secure_stop\n");

   uint8_t sessionId01[] = { 0xa0, 0xa1, 0xa2, 0xa3 };
   unsigned char serverResponse[] = { 0xb0, 0xb1, 0xb2, 0xb3 };

   fprintf(stderr, "About to call opencdm_system_commit_secure_stop\n");
   opencdm_system_ext_commit_secure_stop(systemExt, sessionId01, sizeof(sessionId01), serverResponse, sizeof(serverResponse));
   fprintf(stderr, "Called opencdm_system_commit_secure_stop\n");

   struct OpenCDMSession * session = NULL;
   const uint8_t drmHeader[] = { 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63 };
   uint32_t drmHeaderLength = sizeof(drmHeader);

   fprintf(stderr, "About to call opencdm_create_session, g_callbacks: %p\n", (&g_callbacks));
   opencdm_create_session(accessor, keySystem,
                          (LicenseType)0, // License type is ignored
                          "drmheader", // Init data is a DRM header
                          drmHeader, drmHeaderLength, // DRM header as init data
                          NULL, 0, // No CMDData
                          &g_callbacks, // Callbacks, for example for JSON playlevels string
                          &session // Returned session
                          );
   fprintf(stderr, "Called opencdm_create_session: %p\n", session);

   const std::string testString01 = "{ \"compressed-video\": 42 }";
   PlayLevels pl01;
   pl01.FromString(testString01);
   fprintf(stderr, "Simple playlevel: %u\n", pl01.m_CompressedVideo.Value());

   fprintf(stderr, "About to call opencdm_session_init_decrypt_context_by_kid\n");
   opencdm_session_init_decrypt_context_by_kid(session);
   fprintf(stderr, "Called opencdm_session_init_decrypt_context_by_kid, sleeping for two seconds, to deal with levels\n");

   sleep(2);

   fprintf(stderr, "About to call opencdm_destroy_session_ext\n");
   opencdm_destruct_session(session);
   fprintf(stderr, "Called opencdm_destroy_session_ext\n");

   const uint32_t secureStoreHashSize = 256;
   uint8_t secureStoreHash[secureStoreHashSize];
   memset(secureStoreHash, 0, sizeof(secureStoreHash));

   fprintf(stderr, "About to call opencdm_get_secure_store_hash\n");
   opencdm_get_secure_store_hash_ext(systemExt, secureStoreHash, secureStoreHashSize);
   fprintf(stderr, "Called opencdm_get_secure_store_hash, last entry: 0x%02x\n", secureStoreHash[secureStoreHashSize - 1]);

   fprintf(stderr, "About to call opencdm_delete_secure_store\n");
   opencdm_delete_secure_store(systemExt);
   fprintf(stderr, "Called opencdm_delete_secure_store\n");

   fprintf(stderr, "About to call opencdm_system_teardown\n");
   opencdm_destruct_system(accessor);
   fprintf(stderr, "Called opencdm_system_teardown\n");

}





/*
   // Stess test code:
   for (int j = 0; j < 10; j++) {
      fprintf(stderr, "About to call opencdm_create_session_netflix\n");
      opencdm_create_session_netflix(accessor, &session, sessionId, contentId, contentIdLength,
                                     licenseType, drmHeader, drmHeaderLength);
      fprintf(stderr, "Called opencdm_create_session_netflix: %p\n", session);

      fprintf(stderr, "About to call opencdm_session_get_session_id_netflix\n");
      uint32_t sessionIdNetflix = opencdm_session_get_session_id_netflix(session);
      fprintf(stderr, "Called opencdm_session_get_session_id_netflix: %u\n", sessionIdNetflix);

      uint8_t dataBuffer[] = { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47 };
      uint32_t dataBufferLength = sizeof(dataBuffer);
      uint8_t ivBuffer[] = { 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57 };
      uint32_t ivBufferLength = sizeof(ivBuffer);

      for (int i = 0; i < 100; i++) {
         dataBuffer[0] = (uint8_t)i;
         //fprintf(stderr, "About to call decrypt\n");
         opencdm_session_decrypt(session, dataBuffer, dataBufferLength, ivBuffer, ivBufferLength);
         //fprintf(stderr, "Called decrypt: 0x%02x -> 0x%02x\n", i, (int)dataBuffer[0]);

         int expectedValue = (i + 1) % 256;
         int receivedValue = (int)dataBuffer[0];

         if (expectedValue != receivedValue) {
            fprintf(stderr, ">> ERROR during test decryption: 0x%02x -> 0x%02x (expected: 0x%02x)\n", i, receivedValue, expectedValue);
         }
      }

      fprintf(stderr, "About to call opencdm_destroy_session_netflix\n");
      opencdm_destroy_session_netflix(session);
      fprintf(stderr, "Called opencdm_destroy_session_netflix\n");
   }
*/
