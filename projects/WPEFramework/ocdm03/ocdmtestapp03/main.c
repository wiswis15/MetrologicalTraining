#include <opencdm/open_cdm.h>
#include <opencdm/open_cdm_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	struct OpenCDMAccessor* accessor = opencdm_create_system();
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

   fprintf(stderr, "About to call decrypt\n");
   opencdm_session_decrypt(session, dataBuffer, dataBufferLength, ivBuffer, ivBufferLength);
   fprintf(stderr, "Called decrypt: 0x%02x -> 0x%02x\n", 0x40, (int)dataBuffer[0]);

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
   strcpy(contentId01, "this_is_content_id_01");

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
   fprintf(stderr, "Called opencdm_session_get_content_id, id: %s\n", contentId02);

   fprintf(stderr, "About to call opencdm_session_set_license_type\n");
   opencdm_session_set_license_type(session, OCDM_LICENSE_LIMITED_DURATION);
   fprintf(stderr, "Called opencdm_session_set_license_type\n");

   fprintf(stderr, "About to call opencdm_session_get_license_type\n");
   enum OcdmLicenseType licenseType02 = opencdm_session_get_license_type(session);
   fprintf(stderr, "Called opencdm_session_get_license_type: %u\n", licenseType02);

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



/*
//   struct OpenCDMSystem * system = opencdm_create_system(keySystem);
//   fprintf(stderr, "Created system: %p\n", system);

   uint8_t serverCertificate[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
   uint16_t serverCertificateLength = sizeof(serverCertificate);

   fprintf(stderr, "About to call SetServerCertificate\n");
   opencdm_system_set_server_certificate(accessor, keySystem, serverCertificate, serverCertificateLength);
   fprintf(stderr, "Called SetServerCertificate\n");


   uint32_t licenseType = 42;
   const char initDataType[] = "TypeOfInitData";
   uint8_t initData[] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
   uint16_t initDataLength = sizeof(initData);
   uint8_t CDMData[] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 };
   uint16_t CDMDataLength = sizeof(CDMData);

   struct OpenCDMSession * session = NULL;

   OpenCDMSessionCallbacks callbacks;
   callbacks.process_challenge = ProcessChallenge;
   callbacks.key_update = KeyUpdate;

   fprintf(stderr, "About to call CreateSession\n");
   opencdm_create_session(accessor, keySystem, licenseType, initDataType, initData, initDataLength, CDMData, CDMDataLength, &callbacks, &session);
   fprintf(stderr, "Called CreateSession\n");

   fprintf(stderr, "Created session: %p\n", session);

   fprintf(stderr, "About to call SessionId\n");
   const char * sessionId = opencdm_session_id(session);
   fprintf(stderr, "Session ID: \"%s\"\n", sessionId);


   fprintf(stderr, "About to call Load\n");
   opencdm_session_load(session);
   fprintf(stderr, "Called Load\n");

   fprintf(stderr, "Sleeping so we can deal with the callback messages\n");
   sleep(2);
   fprintf(stderr, "Done sleeping, keyLength: %u\n", (unsigned int)g_keyLength);

/*
   fprintf(stderr, "About to get session\n");
   struct OpenCDMSession * returnedSession = opencdm_get_session(g_keyId, g_keyLength, 300);
   fprintf(stderr, "Original session: %p, returnedSession: %p\n", session, returnedSession);

   fprintf(stderr, "About to call SessionId\n");
   fprintf(stderr, "Returned session, session ID: \"%s\"\n", opencdm_session_session_id(returnedSession));

   fprintf(stderr, "About to call Update\n");
   opencdm_session_update(session, g_keyId, g_keyLength);
   fprintf(stderr, "Called Update\n");

   fprintf(stderr, "About to call Remove\n");
   opencdm_session_remove(session);
   fprintf(stderr, "Called Remove\n");

   fprintf(stderr, "About to call Error\n");
   uint32_t error = opencdm_session_error(session, g_keyId, g_keyLength);
   fprintf(stderr, "Called Error: %u\n", error);

   fprintf(stderr, "About to call SystemError\n");
   OpenCDMError systemError = opencdm_session_system_error(session);
   fprintf(stderr, "Called SystemError: %u\n", systemError);

   uint8_t dataBuffer[] = { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47 };
   uint32_t dataBufferLength = sizeof(dataBuffer);
   uint8_t ivBuffer[] = { 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57 };
   uint32_t ivBufferLength = sizeof(ivBuffer);

   fprintf(stderr, "About to call decrypt\n");
   opencdm_session_decrypt(session, dataBuffer, dataBufferLength, ivBuffer, ivBufferLength);
   fprintf(stderr, "Called decrypt\n");

   fprintf(stderr, "First value: 0x%02X\n", (uint32_t)dataBuffer[0]);
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
