#include <iostream>

#include "globals.h"

#include "../../rpc/Communicator.h"
#include "../../rpc/ITracing.h"

// TODO: this has to be better: the while-null loop should be replaced by a condition variable or something like that.
WPEFramework::RPC::IRemoteProcess * volatile g_process = nullptr;

class MyListener : public WPEFramework::RPC::Communicator::RemoteProcess::INotification
{
   virtual void AddRef() const
   {

   }

   virtual uint32 Release() const
   {
      return 1;
   }

    virtual void *QueryInterface(const uint32 interfaceNumber VARIABLE_IS_NOT_USED)
   {
      return this;
   }

   virtual void Activated(WPEFramework::RPC::IRemoteProcess* process)
   {
      std::cout << "MyListener::Activated: " << process->Id() << std::endl;

      std::cout << "MyListener::Activated, traceiterator interface id: " << WPEFramework::Trace::ITraceIterator::ID << std::endl;

      g_process = process;
   }

    virtual void Deactivated(WPEFramework::RPC::IRemoteProcess *process VARIABLE_IS_NOT_USED)
   {
      std::cout << "MyListener::Deactivated" << std::endl;
   }
};

int main()
{
   WPEFramework::Core::NodeId nodeId(g_hostName.c_str());

   string persistentPath = "/home/sander/Projects/WPEFramework/persistent_path";
   string systemPath = "/home/sander/Projects/WPEFramework/build/Plugins";
   string dataPath = "/home/sander/Projects/WPEFramework/eclipse/src/Plugins/";
   string appPath = "";
   string proxyStubPath = "/home/sander/Projects/WPEFramework/build/ProxyStubs";

   MyListener listener;

   WPEFramework::RPC::Communicator communicator(nodeId, persistentPath, systemPath, dataPath, appPath, proxyStubPath, 0);

   communicator.Register(&listener);

   std::cerr << "Server is sleeping..." << std::endl;
   sleep(10);
   std::cerr << "Server woke up" << std::endl;

   std::list<uint32> listOfPids;
   communicator.Processes(listOfPids);
   std::cout << "Number of processes: " << listOfPids.size() << std::endl;

   std::cout << "Server is waiting for process..." << std::endl;

   while (g_process == nullptr);
   std::cout << "Received pointer to remote process, wait an extra two seconds..." << std::endl;

   sleep(2);
   std::cout << "Trying to instantiate TraceIterator..." << std::endl;
   WPEFramework::Trace::ITraceIterator * traceIterator = g_process->Instantiate<WPEFramework::Trace::ITraceIterator>(WPEFramework::Core::infinite, _T("TraceIterator"), static_cast<uint32>(~0));

   std::cout << "Created TraceIterator..." << std::endl;

   bool enabled;
   string moduleName;
   string categoryName;
   while (traceIterator->Info(enabled, moduleName, categoryName)) {
      std::cout << "Category: " << enabled << " " << moduleName << " " << categoryName << std::endl;
   }

   WPEFramework::Core::Singleton::Dispose();
   return 0;
}
