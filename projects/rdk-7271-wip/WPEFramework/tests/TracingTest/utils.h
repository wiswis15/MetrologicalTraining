#pragma once

#include "../../Source/core/Portability.h"

#include <ctime>

void waitUntilAfterStart(time_t secs);

extern time_t g_startTime;

