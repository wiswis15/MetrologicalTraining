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

