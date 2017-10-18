#ifndef ___UNITTTEST_TIME_HELPERS__
#define ___UNITTTEST_TIME_HELPERS__

#include <generics/Portability.h>

namespace WPEFramework {
namespace Core {
namespace Test {

extern const uint32 NTPToUNIXSeconds;

// Difference in Seconds between UNIX and NTP epoch (25567).
static constexpr uint32 SecondsPerMinute = 60;
static constexpr uint32 MinutesPerHour = 60;
static constexpr uint32 HoursPerDay = 24;
static constexpr uint32 SecondsPerHour = SecondsPerMinute * MinutesPerHour;
static constexpr uint32 SecondsPerDay = SecondsPerHour * HoursPerDay;

static constexpr uint32 MilliSecondsPerSecond = 1000;
static constexpr uint32 MicroSecondsPerMilliSecond = 1000;
static constexpr uint32 NanoSecondsPerMicroSecond = 1000;
static constexpr uint32 MicroSecondsPerSecond = MilliSecondsPerSecond * MicroSecondsPerMilliSecond;
static constexpr uint32 NanoSecondsPerMilliSecond = NanoSecondsPerMicroSecond * MicroSecondsPerMilliSecond;

// Some standard functions to determine expected results. Although these are re-implementations, we cannot currently
// use the ones in Core, as they are static functions in Time.cpp
// Convert tm struct to time_t for GMT. GMT version of mktime, which is not normally implemented on Linux and Windows.
time_t mktimegm(const struct tm *tm);
// Return three letter week day name
const TCHAR * WeekDayName(uint8 weekDayNumber);
// Return three letter month name
const TCHAR * MonthName(uint8 monthNumber);
// Calculate time in seconds and milliseconds as a NTP time (32.32 bits)
uint64 CalculateNTPTime(uint32 seconds, uint32 milliSeconds);
// Calculate time in seconds and milliseconds as ticks (total time in microseconds)
uint64 CalculateTicks(uint32 seconds, uint32 milliSeconds);

} // namespace Test
} // namespace Core
} // namespace WPEFramework

#endif // ___UNITTTEST_TIME_HELPERS__
