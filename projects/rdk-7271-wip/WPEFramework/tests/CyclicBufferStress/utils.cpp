#include "utils.h"

#include <unistd.h>
#include <iostream>

time_t g_startTime = 0;

unsigned int g_maxBufferValue = 256;

void waitUntilAfterStart(time_t secs)
{
   time_t target = g_startTime + secs;

   // Check every 0.1 second if we can continue.
   while (time(NULL) < target) {
      const useconds_t miliSec = 1000;
      usleep(100 * miliSec);
   }
}

void fillBuffer(uint8 buffer[], unsigned int size)
{
   for (unsigned int index = 0; index < size; index++) {
      buffer[index] = static_cast<uint8>(index % g_maxBufferValue);
   }
}

bool checkBuffer(uint8 buffer[], unsigned int size, unsigned int offset /*= 0*/)
{
   for (unsigned int index = 0; index < size; index++) {
      uint8 expectedValue = (index + offset) % g_maxBufferValue;
      uint8 actualValue = buffer[index];

      if (actualValue != expectedValue) {
         std::cerr << "Expected: " << static_cast<unsigned int>(expectedValue)
                   << ", actual: " << static_cast<unsigned int>(actualValue) << std::endl;
         return false;
      }
   }

   return true;
}

