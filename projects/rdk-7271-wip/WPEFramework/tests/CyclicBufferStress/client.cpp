#include "client.h"
#include "utils.h"
#include "globals.h"

#include "../../Source/core/core.h"

#include <iostream>

int runClient()
{
   WPEFramework::Core::CyclicBuffer buffer(g_bufferFilePath, 0, true);
   if (buffer.IsValid() == false) {
      std::cerr << "Client failed to open Memory Mapped File, error: " << buffer.ErrorCode() << std::endl;
      return 1;
   }

   // Wait three seconds, then read from buffer twice.
   waitUntilAfterStart(3);

   uint8 loadBuffer01[128];
   uint32 result = buffer.Read(loadBuffer01, sizeof(loadBuffer01) - 1);
   std::cerr << "2. Client read " << result << " bytes from buffer" << std::endl;

   result = buffer.Read(loadBuffer01, sizeof(loadBuffer01) - 1);
   std::cerr << "3. Client read " << result << " bytes from buffer" << std::endl;

   // Wait two seconds.
   waitUntilAfterStart(5);
   
   // Fill cyclic buffer entirely.
   uint8 writeBuffer02[g_bufferSize];
   fillBuffer(writeBuffer02, g_bufferSize);

   buffer.Write(writeBuffer02, sizeof(writeBuffer02) - 1);
   std::cerr << "4. Client wrote " << g_bufferSize - 1 << " bytes to buffer" << std::endl;
   
   // Wait two more seconds.
   waitUntilAfterStart(7);

   // Write too large a buffer.
   uint8 writeBuffer03[2 * g_bufferSize];
   fillBuffer(writeBuffer03, sizeof(writeBuffer03));

   unsigned int totalWritten01 = 0;
   unsigned int writtenSize = 200;
   buffer.Write(writeBuffer03, writtenSize);

   totalWritten01 += writtenSize;
   
   writtenSize = 100;
   result = buffer.Write(writeBuffer03 + totalWritten01, writtenSize);
   totalWritten01 += writtenSize;

   std::cerr << "6. Client wrote " << totalWritten01 << " bytes to buffer" << std::endl;
   
   waitUntilAfterStart(11);

   return 0;
}
