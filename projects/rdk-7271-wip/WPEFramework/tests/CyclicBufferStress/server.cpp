#include "server.h"
#include "utils.h"
#include "globals.h"

#include "../../Source/core/core.h"

#include <iostream>

const char g_defaultText[] = "the lazy string jumped over the quick brown buffer";

int runServer()
{
   WPEFramework::Core::CyclicBuffer buffer(g_bufferFilePath, g_bufferSize, true);
   if (buffer.IsValid() == false) {
      std::cerr << "Server failed to open Memory Mapped File, error: " << buffer.ErrorCode() << std::endl;
      return 1;
   }
   
   // Wait two seconds, then write the string to buffer.
   waitUntilAfterStart(2);
   buffer.Write(reinterpret_cast<const uint8*>(g_defaultText), sizeof(g_defaultText));
   std::cerr << "1. Server wrote " << sizeof(g_defaultText) << " bytes to buffer" << std::endl;

   // Wait four more seconds.
   waitUntilAfterStart(6);
   
   // Read whole buffer.
   uint8 readBuffer01[g_bufferSize];
   uint32 result = buffer.Read(readBuffer01, sizeof(readBuffer01));
   
   if (!checkBuffer(readBuffer01, result)) {
      std::cerr << "ERROR: read buffer was incorrect." << std::endl; 
      return 1;
   }
   
   std::cerr << "5. Server read " << result << " bytes from buffer" << std::endl;

   // Wait two more seconds.
   waitUntilAfterStart(8);

   result = buffer.Read(readBuffer01, sizeof(readBuffer01));
   if (!checkBuffer(readBuffer01, result, 300 - g_bufferSize + 1)) {
      std::cerr << "ERROR: read buffer was incorrect." << std::endl; 
      return 1;
   }
   
   std::cerr << "7. Server read " << result << " bytes from buffer" << std::endl;

   // Wait two more seconds.
   waitUntilAfterStart(10);

   return 0;
}

