#include "ResourceMonitorImplementation.cpp"

#include <core/WorkerPool.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace WPEFramework;

int main(int argc, const char ** argv)
{
   if (argc != 6) {
      cerr << "Usage: " << argv[0] << " [bin-dump-path] [interval (sec)] [mode] [parent-name] [output-name]" << endl;
      cerr << "Valid modes: \"single\", \"multiple\", \"callsign\", \"classname\"" << endl;
      cerr << "Example:" << endl;
      cerr << "   " << argv[0] << " /tmp/resource-log.bin 5 single WPEFramework-1.0.0 measurement.csv" << endl;
      Core::Singleton::Dispose();
      return 1;
   }

   Core::WorkerPool workerPool(4, 64 * 1024, 16);
   Core::WorkerPool::Assign(&workerPool);

   string dumpPath = argv[1];
   int interval = atoi(argv[2]);
   string mode = argv[3];
   string parentName = argv[4];
   string output = argv[5];

   Plugin::ResourceMonitorImplementation monitor;
   monitor.Configure(dumpPath, interval, mode, parentName);

   while (true) {
      ofstream out(output.c_str());
      string csv = monitor.CompileMemoryCsv();
      out << csv << flush;
      out.close();
      cout << "Dumped to " << output << endl;
      sleep(interval);
   }

   Core::Singleton::Dispose();
}
