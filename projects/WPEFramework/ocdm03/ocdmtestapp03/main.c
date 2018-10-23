#include <opencdm/open_cdm.h>
#include <opencdm/open_cdm_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	//struct OpenCDMAccessor* accessor = opencdm_create_system();
   struct OpenCDMAccessor* accessor = opencdm_create_system_netflix("", "");
   fprintf(stderr, "Created system: %p\n", accessor);

   //const char keySystem[] = "com.metrological.null";
   const char keySystem[] = "com.microsoft.playready";
   OpenCDMError isSupported = opencdm_is_type_supported(accessor, keySystem, "");
   fprintf(stderr, "com.metrological.null is supported (should be 0): %u\n", isSupported);

   isSupported = opencdm_is_type_supported(accessor, "foobar", "");
   fprintf(stderr, "foobar is supported (should be != 0): %u\n", isSupported);
   
   time_t drmSystemTime;

   fprintf(stderr, "About to call opencdm_system_get_drm_time\n");
   opencdm_system_get_drm_time(accessor, &drmSystemTime);
   fprintf(stderr, "Called opencdm_system_get_drm_time: %lu\n", drmSystemTime);

   char versionStr01[65];
   fprintf(stderr, "About to call opencdm_system_get_version\n");
   opencdm_system_get_version(accessor, versionStr01);
   fprintf(stderr, "Called opencdm_system_get_version: %s\n", versionStr01);

   uint32_t ldlLimit01;
   fprintf(stderr, "About to call opencdm_system_get_ldl_session_limit\n");
   opencdm_system_get_ldl_session_limit(accessor, &ldlLimit01);
   fprintf(stderr, "Called opencdm_system_get_ldl_session_limit: %u\n", ldlLimit01);

   fprintf(stderr, "About to call opencdm_system_enable_secure_stop\n");
   opencdm_system_enable_secure_stop(accessor, 1);
   fprintf(stderr, "Called opencdm_system_enable_secure_stop\n");

   uint8_t sessionId01[] = { 0xa0, 0xa1, 0xa2, 0xa3 };
   unsigned char serverResponse[] = { 0xb0, 0xb1, 0xb2, 0xb3 };

   fprintf(stderr, "About to call opencdm_system_commit_secure_stop\n");
   opencdm_system_commit_secure_stop(accessor, sessionId01, sizeof(sessionId01), serverResponse, sizeof(serverResponse));
   fprintf(stderr, "Called opencdm_system_commit_secure_stop\n");

   struct OpenCDMSession * session = NULL;
   uint32_t sessionId = 61;
   const char contentId[] = "content_id_62";
   uint32_t contentIdLength = strlen(contentId) + 1;
   enum OcdmLicenseType licenseType = OCDM_LICENSE_LIMITED_DURATION;
   const uint8_t drmHeader[] = { 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63 };
   uint32_t drmHeaderLength = sizeof(drmHeader);

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
   unsigned long long byteOffset = 3;

//   fprintf(stderr, "About to call decrypt\n");
//   opencdm_session_decrypt(session, dataBuffer, dataBufferLength, ivBuffer, ivBufferLength);
//   fprintf(stderr, "Called decrypt: 0x%02x -> 0x%02x\n", 0x40, (int)dataBuffer[0]);

   fprintf(stderr, "About to call opencdm_session_decrypt_netflix\n");
   opencdm_session_decrypt_netflix(session, ivBuffer, ivBufferLength, byteOffset, dataBuffer, dataBufferLength);
   fprintf(stderr, "Called opencdm_session_decrypt_netflix: 0x%02x -> 0x%02x\n", 0x40, (int)dataBuffer[0]);

   fprintf(stderr, "About to call opencdm_session_get_playlevel_compressed_video\n");
   uint16_t playLevel01 = opencdm_session_get_playlevel_compressed_video(session);
   fprintf(stderr, "Called opencdm_session_get_playlevel_compressed_video: %u\n", playLevel01);

   fprintf(stderr, "About to call opencdm_session_get_playlevel_uncompressed_video\n");
   playLevel01 = opencdm_session_get_playlevel_uncompressed_video(session);
   fprintf(stderr, "Called opencdm_session_get_playlevel_uncompressed_video: %u\n", playLevel01);

   fprintf(stderr, "About to call opencdm_session_get_playlevel_analog_video\n");
   playLevel01 = opencdm_session_get_playlevel_analog_video(session);
   fprintf(stderr, "Called opencdm_session_get_playlevel_analog_video: %u\n", playLevel01);

   fprintf(stderr, "About to call opencdm_session_get_playlevel_compressed_audio\n");
   playLevel01 = opencdm_session_get_playlevel_compressed_audio(session);
   fprintf(stderr, "Called opencdm_session_get_playlevel_compressed_audio: %u\n", playLevel01);

   fprintf(stderr, "About to call opencdm_session_get_playlevel_uncompressed_audio\n");
   playLevel01 = opencdm_session_get_playlevel_uncompressed_audio(session);
   fprintf(stderr, "Called opencdm_session_get_playlevel_uncompressed_audio: %u\n", playLevel01);

   char contentId01[30];
   memset(contentId01, 0, sizeof(contentId01));
   strcpy(contentId01, "this_is_content_id_01");

   int extraCharIndex = strlen(contentId01) + 2;
   contentId01[extraCharIndex] = 'a'; // Netflix uses content IDs that aren't valid UTF-8 strings.

   fprintf(stderr, "About to call opencdm_session_set_content_id\n");
   opencdm_session_set_content_id(session, contentId01, sizeof(contentId01));
   fprintf(stderr, "Called opencdm_session_set_content_id\n");

   uint32_t bufferSize = 0;
   fprintf(stderr, "About to call opencdm_session_get_content_id (1)\n");
   opencdm_session_get_content_id(session, NULL, &bufferSize);
   fprintf(stderr, "Called opencdm_session_get_content_id, size: %u\n", bufferSize);

   char * contentId02 = malloc(bufferSize);
   fprintf(stderr, "About to call opencdm_session_get_content_id (2)\n");
   opencdm_session_get_content_id(session, contentId02, &bufferSize);
   fprintf(stderr, "Called opencdm_session_get_content_id, id: %s, extra char: %c\n", contentId02, contentId02[extraCharIndex]);

   fprintf(stderr, "About to call opencdm_session_set_license_type\n");
   opencdm_session_set_license_type(session, OCDM_LICENSE_LIMITED_DURATION);
   fprintf(stderr, "Called opencdm_session_set_license_type\n");

   fprintf(stderr, "About to call opencdm_session_get_license_type\n");
   enum OcdmLicenseType licenseType02 = opencdm_session_get_license_type(session);
   fprintf(stderr, "Called opencdm_session_get_license_type: %u\n", licenseType02);

   enum OcdmSessionState sessionState01 = LicenseAcquisitionState;

   fprintf(stderr, "About to call opencdm_session_set_session_state\n");
   opencdm_session_set_session_state(session, sessionState01);
   fprintf(stderr, "Called opencdm_session_set_session_state\n");

   fprintf(stderr, "About to opencdm_session_get_session_state\n");
   enum OcdmSessionState sessionState02 = opencdm_session_get_session_state(session);
   fprintf(stderr, "Called opencdm_session_get_session_state: %u <-> %u\n", sessionState01, sessionState02);

   sessionState01 = InactiveDecryptionState;
   fprintf(stderr, "About to call opencdm_session_set_session_state\n");
   opencdm_session_set_session_state(session, sessionState01);
   fprintf(stderr, "Called opencdm_session_set_session_state\n");

   fprintf(stderr, "About to opencdm_session_get_session_state\n");
   sessionState02 = opencdm_session_get_session_state(session);
   fprintf(stderr, "Called opencdm_session_get_session_state: %u <-> %u\n", sessionState01, sessionState02);

   sessionState01 = ActiveDecryptionState;
   fprintf(stderr, "About to call opencdm_session_set_session_state\n");
   opencdm_session_set_session_state(session, sessionState01);
   fprintf(stderr, "Called opencdm_session_set_session_state\n");

   fprintf(stderr, "About to opencdm_session_get_session_state\n");
   sessionState02 = opencdm_session_get_session_state(session);
   fprintf(stderr, "Called opencdm_session_get_session_state: %u <-> %u\n", sessionState01, sessionState02);

   sessionState01 = InvalidState;
   fprintf(stderr, "About to call opencdm_session_set_session_state\n");
   opencdm_session_set_session_state(session, sessionState01);
   fprintf(stderr, "Called opencdm_session_set_session_state\n");

   fprintf(stderr, "About to opencdm_session_get_session_state\n");
   sessionState02 = opencdm_session_get_session_state(session);
   fprintf(stderr, "Called opencdm_session_get_session_state: %u <-> %u\n", sessionState01, sessionState02);

   uint8_t drmHeader01[] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77 };

   fprintf(stderr, "About to call opencdm_session_set_drm_header\n");
   opencdm_session_set_drm_header(session, drmHeader01, sizeof(drmHeader01));
   fprintf(stderr, "Called opencdm_session_set_drm_header\n");

   uint32_t challengeSize01 = 0;
   fprintf(stderr, "About to call opencdm_session_get_challenge_data_netflix (1)\n");
   opencdm_session_get_challenge_data_netflix(session, NULL, &challengeSize01, 1);
   fprintf(stderr, "Called opencdm_session_get_challenge_data_netflix, size: %u\n", challengeSize01);

   uint8_t * challengeData01 = malloc(challengeSize01);
   fprintf(stderr, "About to call opencdm_session_get_challenge_data_netflix (2)\n");
   opencdm_session_get_challenge_data_netflix(session, challengeData01, &challengeSize01, 1);
   fprintf(stderr, "Called opencdm_session_get_challenge_data_netflix, data[0]: 0x%02x\n", challengeData01[0]);

   uint8_t licenseData01[] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87 };
   unsigned char secureStopId[16];
   memset(secureStopId, 0, sizeof(secureStopId));

   fprintf(stderr, "About to call opencdm_session_store_license_data, id: %p\n", secureStopId);
   opencdm_session_store_license_data(session, licenseData01, sizeof(licenseData01), secureStopId);
   fprintf(stderr, "Called opencdm_session_store_license_data, last entry: 0x%02x\n", secureStopId[15]);

   fprintf(stderr, "About to call opencdm_session_init_decrypt_context_by_kid\n");
   opencdm_session_init_decrypt_context_by_kid(session);
   fprintf(stderr, "Called opencdm_session_init_decrypt_context_by_kid\n");

   fprintf(stderr, "About to call opencdm_destroy_session_netflix\n");
   opencdm_destroy_session_netflix(session);
   fprintf(stderr, "Called opencdm_destroy_session_netflix\n");

   const uint32_t secureStoreHashSize = 256;
   uint8_t secureStoreHash[secureStoreHashSize];
   memset(secureStoreHash, 0, sizeof(secureStoreHash));

   fprintf(stderr, "About to call opencdm_get_secure_store_hash\n");
   opencdm_get_secure_store_hash(accessor, secureStoreHash, secureStoreHashSize);
   fprintf(stderr, "Called opencdm_get_secure_store_hash, last entry: 0x%02x\n", secureStoreHash[secureStoreHashSize - 1]);

   fprintf(stderr, "About to call opencdm_delete_secure_store\n");
   opencdm_delete_secure_store(accessor);
   fprintf(stderr, "Called opencdm_delete_secure_store\n");

   fprintf(stderr, "About to call opencdm_system_teardown\n");
   opencdm_system_teardown(accessor);
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
