#include <opencdm/open_cdm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <iostream>

using namespace media;
using namespace std;

/*
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
*/

int main()
{
   OpenCdm & openCDM = OpenCdm::Instance();

   const string keySystem = "com.microsoft.playready";
   bool supported = openCDM.IsTypeSupported(keySystem, "");

   cout << "Supported: " << supported << endl;

   cerr << "About to select keysystem" << endl;
   openCDM.SelectKeySystem(keySystem);
   cerr << "Selected keysystem" << endl;

   uint8_t addData01[] = { 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63 };
   uint16_t addData01Length = sizeof(addData01);
   uint8_t cdmData01[] = { 0x73, 0x73, 0x73, 0x73, 0x73, 0x73, 0x73, 0x73 };
   uint16_t cdmData01Length = sizeof(cdmData01);
   // TODO: what is first arg?
   cerr << "About to create session" << endl;
   string sessionId = openCDM.CreateSession("", addData01, addData01Length, cdmData01, cdmData01Length, OpenCdm::PersistentLicense);
   cerr << "Created session" << endl;

   string message;
   uint8_t temporaryURL[1024];
   uint16_t temporaryURLLength = sizeof(temporaryURL);
   cerr << "About to get key message" << endl;
   openCDM.GetKeyMessage(message, temporaryURL, temporaryURLLength);
   cerr << "Got key message" << endl;

   uint8_t updateData01[] = { 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83 };
   uint16_t updateData01Length = sizeof(updateData01);
   string output01;

   cerr << "About to update" << endl;
   openCDM.Update(updateData01, updateData01Length, output01);
   cerr << "Updated" << endl;

   cerr << "About to get OpenCdm by session id" << endl;
   OpenCdm openCDMById(sessionId);
   cerr << "Got OpenCdm by session id" << endl;

   uint8_t encryptedData01[] = { 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93 };
   uint16_t encryptedData01Length = sizeof(encryptedData01);
   uint8_t ivData01[] = { 0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3 };
   uint16_t ivData01Length = sizeof(ivData01);
   //uint8_t keyId01[] = { 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf };
   //uint16_t keyIdLength = sizeof(keyId01);
	uint8_t keyId01[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f };
	uint32_t keyIdLength = sizeof(keyId01);
   uint32_t initWithLast15 = 0;
   cerr << "About to decrypt" << endl;
   openCDMById.Decrypt(encryptedData01, encryptedData01Length, ivData01, ivData01Length, keyIdLength, keyId01, initWithLast15);
   //openCDMById.Decrypt(encryptedData01, encryptedData01Length, ivData01, ivData01Length, sessionId.length(), reinterpret_cast<const uint8_t*>(sessionId.c_str()), initWithLast15);
   cerr << "Decrypted" << endl;

   cerr << "End of main" << endl;

/*
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
*/   

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
