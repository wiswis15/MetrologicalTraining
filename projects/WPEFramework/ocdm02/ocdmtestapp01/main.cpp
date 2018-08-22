#include <ocdm-accessor/AccessorOCDM.h>
#include <ocdm/IOCDM.h>

#include <iostream>
#include <string>

using namespace std;

// To store key ID we get in callback.
uint8_t * g_keyId = nullptr;
uint8_t g_keyLength = 0;

class MyCallback: public OCDM::ISession::ICallback {
public:
	void ProcessChallenge(const std::string& URL, const uint8_t challenge[], const uint16_t challengeLength) override
	{
		cerr << "MyCallback::ProcessChallenge, url: " << URL << ", challenge length: " << challengeLength << endl;
	}

	void KeyUpdate(const uint8_t keyId[], const uint8_t length) override
	{
		cerr << "MyCallback::KeyUpdate, length: " << static_cast<uint32_t>(length) << endl;

		g_keyId = new uint8_t[length];
		memcpy(g_keyId, keyId, length);
		g_keyLength = length;
	}

    BEGIN_INTERFACE_MAP(MyCallback)
        INTERFACE_ENTRY(OCDM::ISession::ICallback)
    END_INTERFACE_MAP
};

int main()
{
   OCDM::IAccessor * accessor = media::AccessorOCDM::Instance();
   
   //string keySystem = "com.microsoft.playready";

   string keySystem = "com.metrological.null";
   WPEFramework::Core::Error isSupported = accessor->IsTypeSupported(keySystem, "");
   fprintf(stderr, "com.metrological.null is supported (should be 0): %u\n", isSupported);

   isSupported = accessor->IsTypeSupported("foobar", "");
   fprintf(stderr, "foobar is supported (should be 1): %u\n", isSupported);
   
   OCDM::ISystem * system = accessor->CreateSystem(keySystem);
   fprintf(stderr, "Created system: %p\n", system);
   
   uint8_t serverCertificate[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
   uint16_t serverCertificateLength = sizeof(serverCertificate);

   cerr << "About to call SetServerCertificate" << endl;
   system->SetServerCertificate(serverCertificate, serverCertificateLength);
   cerr << "Called SetServerCertificate" << endl;

   uint32_t licenseType = 42;
   string initDataType = "TypeOfInitData";
   uint8_t initData[] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
   uint16_t initDataLength = sizeof(initData);
   uint8_t CDMData[] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 };
   uint16_t CDMDataLength = sizeof(CDMData);

   OCDM::ISession * session = nullptr;

   cerr << "About to call CreateSession" << endl;
   system->CreateSession(licenseType, initDataType, initData, initDataLength, CDMData, CDMDataLength, session);
   cerr << "Called CreateSession" << endl;

   fprintf(stderr, "Created session: %p\n", session);

   cerr << "About to call SessionId" << endl;
   string sessionId = session->SessionId();
   cerr << "Session ID: \"" << sessionId << "\"" << endl;

   cerr << "About to call Load" << endl;
   session->Load();
   cerr << "Called Load" << endl;

   OCDM::ISession::ICallback * myCallback = Core::Service<MyCallback>::Create<OCDM::ISession::ICallback>();
   cerr << "About to call Callback" << endl;
   session->Callback(myCallback);
   cerr << "Called Callback" << endl;

   cerr << "Sleeping so we can deal with the callback messages" << endl;
   sleep(2);
   cerr << "Done sleeping, keyLength: " << (int)g_keyLength << endl;

   cerr << "About to get session" << endl;
   OCDM::ISession * returnedSession = accessor->Session(g_keyId, g_keyLength, 3000);
   cerr << "Original session: " << session << ", returnedSession: " << returnedSession << endl;

   cerr << "About to call SessionId" << endl;
   cerr << "Returned session, session ID: " << returnedSession->SessionId() << endl;

   cerr << "About to call Update" << endl;
   session->Update(g_keyId, g_keyLength);
   cerr << "Called Update" << endl;

   cerr << "About to call Remove" << endl;
   session->Remove();
   cerr << "Called Remove" << endl;

   cerr << "About to call Error" << endl;
   uint32_t error = session->Remove();
   cerr << "Called Error: " << error << endl;

   returnedSession->Release();
   session->Release();
   system->Release();
   accessor->Release();


/*
   time_t drmSystemTime = ocdm->GetSystemTime();

   cerr << "DRM time: " << drmSystemTime << endl;
*/

   cerr << "\n\n\n\n\n" << endl;


   WPEFramework::Core::Singleton::Dispose();
}

