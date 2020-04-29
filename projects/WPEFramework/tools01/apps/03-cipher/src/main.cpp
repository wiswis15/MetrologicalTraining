#include <core/core.h>
#include <com/com.h>
#include <iostream>
#include <inttypes.h>
#include <cryptography/cryptography.h>
#include <plugins/plugins.h>

using namespace std;
using namespace WPEFramework;

int main()
{


    //Cryptography::ICryptography* cryptography = Cryptography::ICryptography::Instance("/tmp/communicator");

//    Cryptography::INetflixSecurity * netflixSec = vault->QueryInterface<Cryptography::INetflixSecurity>();
//    cerr << netflixSec << endl;

    WPEFramework::Cryptography::INetflixSecurity* nfSecurity = WPEFramework::Cryptography::INetflixSecurity::Instance();
    cout << "nfSecurity: " << nfSecurity << endl;

    Cryptography::ICryptography* cryptography = Cryptography::ICryptography::Instance("");
    cerr << cryptography << endl;

    Cryptography::IVault * vault = cryptography->Vault(CRYPTOGRAPHY_VAULT_NETFLIX);
    cerr << "vault = " << vault << endl;

    //nfSecurity

    cout << "ESN = >>" << nfSecurity->ESN() << "<<" << endl;


/*
    Core::NodeId nodeId("/tmp/communicator");

    Core::ProxyType< RPC::InvokeServerType<1, 0, 4> > engine(
        Core::ProxyType<RPC::InvokeServerType<1, 0, 4>>::Create());

    Core::ProxyType<RPC::CommunicatorClient> client(
        Core::ProxyType<RPC::CommunicatorClient>::Create(
            nodeId,
            Core::ProxyType< Core::IIPCServer >(engine)));

    engine->Announcements(client->Announcement());

    //PluginHost::IShell* controller = client->Open<PluginHost::IShell>(_T("Dictionary"), ~0, 3000);
    PluginHost::IPlugin* controller = client->Open<PluginHost::IPlugin>(_T("Controller"), ~0, 3000);
    //Cryptography::INetflixSecurity* ns = client->Open<Cryptography::INetflixSecurity>(_T("NetflixSecurity"), ~0, 3000);
    //Cryptography::ICryptography* c01 = client->Open<Cryptography::ICryptography>(_T("CryptographyImpl"), ~0, 3000);

    //cerr << "ns: " << ns << endl;
    //cerr << "c01: " << c01 << endl;
    cerr << "controller: " << controller << endl;
*/

/*
    const string connectorName = "/tmp/communicator";
    Core::NodeId remoteNode(connectorName.c_str());

    //Core::ProxyType<WorkerPoolImplementation> _engine;
    Core::ProxyType<Core::WorkerPool> _engine;
    uint8_t threadCount = 4;
    //_engine = Core::ProxyType<WorkerPoolImplementation>::Create(threadCount, Core::Thread::DefaultStackSize(), 16);
    _engine = Core::ProxyType<Core::WorkerPool>::Create(threadCount, Core::Thread::DefaultStackSize(), 16);

    Core::IWorkerPool::Assign(&(*_engine));

    Core::ProxyType<RPC::CommunicatorClient> _server;
    _server = (Core::ProxyType<RPC::CommunicatorClient>::Create(remoteNode, Core::ProxyType<Core::IIPCServer>(_engine)));
    //_engine->Announcements(_server->Announcement());

    Exchange::IAdder * adder = client->Open<Exchange::IAdder>(_T("Adder"));

    cout << 
*/


/*
    //Core::ProxyType<RPC::InvokeServerType<4, 1, 8>> engine(Core::ProxyType<RPC::InvokeServerType<4, 1, 8>>::Create());

    Core::ProxyType<Core::IIPCServer> engine(Core::ProxyType<RPC::InvokeServer>::Create(&Core::WorkerPool::Instance()));
    
    Core::ProxyType<RPC::CommunicatorClient>::Create(remoteNode, engine);
    //Core::ProxyType<RPC::CommunicatorClient> client(Core::ProxyType<RPC::CommunicatorClient>::Create(engine));
    */

/*
    Core::ProxyType<RPC::CommunicatorClient> client(
        Core::ProxyType<RPC::CommunicatorClient>::Create(
            remoteNode,
            Core::ProxyType<Core::IIPCServer>(engine)
        ));
*/


/*
    engine->Announcements(client->Announcement());
*/

    // Exchange::IAdder * adder = client->Open<Exchange::IAdder>(_T("Adder"));

    Core::Singleton::Dispose();
}
