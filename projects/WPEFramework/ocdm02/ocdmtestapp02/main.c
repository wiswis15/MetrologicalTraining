#include <ocdm-accessor/open_cdm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// To store key ID we get in callback.
uint8_t * g_keyId = NULL;
uint8_t g_keyLength = 0;

// Callbacks
static void ProcessChallenge(void * userData, const char url[], const uint8_t challenge[], uint16_t challengeLength)
{
	fprintf(stderr, "MyCallback::ProcessChallenge, url: %s, challenge length: %u\n", url, challengeLength);
}

static void KeyUpdate(void * userData, const uint8_t keyId[], uint8_t length)
{
	fprintf(stderr, "MyCallback::KeyUpdate, length: %u\n", (unsigned int)length);

	g_keyId = malloc(length);
	memcpy(g_keyId, keyId, length);
	g_keyLength = length;
}

int main()
{
   const char keySystem[] = "com.metrological.null";
   OpenCDMError isSupported = opencdm_is_type_supported(keySystem, "");
   fprintf(stderr, "com.metrological.null is supported (should be 0): %u\n", isSupported);

   isSupported = opencdm_is_type_supported("foobar", "");
   fprintf(stderr, "foobar is supported (should be 1): %u\n", isSupported);
   
   struct OpenCDMSystem * system = opencdm_create_system(keySystem);
   fprintf(stderr, "Created system: %p\n", system);

   uint8_t serverCertificate[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
   uint16_t serverCertificateLength = sizeof(serverCertificate);

   fprintf(stderr, "About to call SetServerCertificate\n");
   opencdm_system_set_server_certificate(system, serverCertificate, serverCertificateLength);
   fprintf(stderr, "Called SetServerCertificate\n");

   uint32_t licenseType = 42;
   const char initDataType[] = "TypeOfInitData";
   uint8_t initData[] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
   uint16_t initDataLength = sizeof(initData);
   uint8_t CDMData[] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 };
   uint16_t CDMDataLength = sizeof(CDMData);

   struct OpenCDMSession * session = NULL;

   fprintf(stderr, "About to call CreateSession\n");
   opencdm_system_create_session(system, licenseType, initDataType, initData, initDataLength, CDMData, CDMDataLength, &session);
   fprintf(stderr, "Called CreateSession\n");

   fprintf(stderr, "Created session: %p\n", session);

   fprintf(stderr, "About to call SessionId\n");
   const char * sessionId = opencdm_session_session_id(session);
   fprintf(stderr, "Session ID: \"%s\"\n", sessionId);

   fprintf(stderr, "About to call Load\n");
   opencdm_session_load(session);
   fprintf(stderr, "Called Load\n");

   OpenCDMSessionCallbacks callbacks;
   callbacks.process_challenge = ProcessChallenge;
   callbacks.key_update = KeyUpdate;

   fprintf(stderr, "About to call Callback\n");
   opencdm_session_callback(session, &callbacks, NULL);
   fprintf(stderr, "Called Callback\n");

   fprintf(stderr, "Sleeping so we can deal with the callback messages\n");
   sleep(2);
   fprintf(stderr, "Done sleeping, keyLength: %u\n", (unsigned int)g_keyLength);

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


}

