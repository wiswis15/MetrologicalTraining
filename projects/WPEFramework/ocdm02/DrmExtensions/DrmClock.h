#pragma once

#include <time.h>

namespace drm {

struct DrmClock
{
   virtual time_t GetSystemTime() const = 0;
};

}

