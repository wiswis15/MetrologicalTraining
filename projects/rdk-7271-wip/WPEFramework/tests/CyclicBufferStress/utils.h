#pragma once

#include "../../Source/core/Portability.h"

#include <ctime>

void waitUntilAfterStart(time_t secs);
void fillBuffer(uint8 buffer[], unsigned int size);
bool checkBuffer(uint8 buffer[], unsigned int size, unsigned int offset = 0);

extern time_t g_startTime;

