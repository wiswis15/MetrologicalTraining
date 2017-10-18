#include "client.h"
#include "server.h"
#include "utils.h"
#include "globals.h"

#include "../../Source/core/core.h"
#include "../../Source/core/Portability.h"

#include <iostream>
#include <ctime>

// When an instance of this class will go out of scope, it will call Dispose on Singleton
class SingletonTrigger
{
public:
   ~SingletonTrigger() {
      WPEFramework::Core::Singleton::Dispose();
   }
};

void PrintUsage(const char appName[])
{
   std::cerr << "Usage: " << appName << " [-client|-server|-time|-path] {start-time}" << std::endl;
}

int main(int argc, const char ** argv)
{
   // Make sure Singleton::Dispose() will be called when main is exited. 
   SingletonTrigger st;

   if (argc == 1) {
      PrintUsage(argv[0]);
      return 1;
   }
   
   string argument = argv[1];
   
   if (argument == "-time") {
      // Script asked for current time in seconds since epoch start.
      time_t t = time(NULL);
      std::cout << t << std::endl;
      return 0;
   } else if (argument == "-path") {
      // Script asked for buffer mem-mapped file path.
      std::cout << g_bufferFilePath << std::endl;
      return 0;
   } else if (argument == "-server") {
      // This instance of stress app will act as server.
      if (argc < 3) {
         PrintUsage(argv[0]);
         return 1;
      }

      g_startTime = atol(argv[2]);
      return runServer();
   } else if (argument == "-client") {
      // This instance of stress app will act as client.
      if (argc < 3) {
         PrintUsage(argv[0]);
         return 1;
      }

      g_startTime = atol(argv[2]);
      return runClient();
   } else {
      PrintUsage(argv[0]);
      return 1;
   }

   return 0;
}
