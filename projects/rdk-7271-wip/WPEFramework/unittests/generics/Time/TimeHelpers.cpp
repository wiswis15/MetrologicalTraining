#include "TimeHelpers.h"

#include <cassert>

namespace WPEFramework {
namespace Core {
namespace Test {

// We are purposely setting to wintertime so we have a one hour time difference.
// Need to check for regional issues though
// Start day of NTP time as days past the imaginary date 12/1/1 BC.
// (This is the beginning of the Christian Era, or BCE.)
static constexpr uint32 DayNTPStarts = 693596;

// Start day of the UNIX epoch (1970-01-01), also counting from BCE
static constexpr uint32 DayUNIXEpochStarts = 719163;

const uint32 NTPToUNIXSeconds =
    (DayUNIXEpochStarts - DayNTPStarts) * SecondsPerDay;

static const uint64 Bit32AsInteger = 4294967296LL;
static const uint32 YearANSI = 1994;
static const uint8 MonthANSI = 11;
static const uint8 DayANSI = 6;
static const uint8 HourANSI = 8;
static const uint8 MinuteANSI = 49;
static const uint8 SecondANSI = 37;
static const uint32 EpochTimestampANSIGMT = 784111777;

// GMT specific version of mktime(), taken from BSD source code
/* Number of days per month (except for February in leap years). */
static const int monoff[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

static int is_leap_year(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static int leap_days(int y1, int y2)
{
    --y1;
    --y2;
    return (y2/4 - y1/4) - (y2/100 - y1/100) + (y2/400 - y1/400);
}

time_t mktimegm(const struct tm *tm)
{
    int year;
    time_t days;
    time_t hours;
    time_t minutes;
    time_t seconds;

    year = 1900 + tm->tm_year;
    days = 365 * (year - 1970) + leap_days(1970, year);
    days += monoff[tm->tm_mon];

    if (tm->tm_mon > 1 && is_leap_year(year))
        ++days;
    days += tm->tm_mday - 1;

    hours = days * 24 + tm->tm_hour;
    minutes = hours * 60 + tm->tm_min;
    seconds = minutes * 60 + tm->tm_sec;

    return seconds;
}

const TCHAR * WeekDayName(uint8 weekDayNumber)
{
    const TCHAR* weekDay = _T("");
    switch (weekDayNumber) {
        case 0:
            weekDay = _T("Sun");
            break;
        case 1:
            weekDay = _T("Mon");
            break;
        case 2:
            weekDay = _T("Tue");
            break;
        case 3:
            weekDay = _T("Wed");
            break;
        case 4:
            weekDay = _T("Thu");
            break;
        case 5:
            weekDay = _T("Fri");
            break;
        case 6:
            weekDay = _T("Sat");
            break;
        default:
            assert(false);
    }
    return weekDay;
}

const TCHAR * MonthName(uint8 monthNumber)
{
    const TCHAR* month;
    switch (monthNumber) {
        case 0:
            month = _T("Jan");
            break;
        case 1:
            month = _T("Feb");
            break;
        case 2:
            month = _T("Mar");
            break;
        case 3:
            month = _T("Apr");
            break;
        case 4:
            month = _T("May");
            break;
        case 5:
            month = _T("Jun");
            break;
        case 6:
            month = _T("Jul");
            break;
        case 7:
            month = _T("Aug");
            break;
        case 8:
            month = _T("Sep");
            break;
        case 9:
            month = _T("Oct");
            break;
        case 10:
            month = _T("Nov");
            break;
        case 11:
            month = _T("Dec");
            break;
        default:
            assert(false);
    }
    return month;
}

uint64 CalculateNTPTime(uint32 seconds, uint32 milliSeconds)
{
    return (static_cast<uint64>(seconds + NTPToUNIXSeconds) << 32) +
           static_cast<uint32>(milliSeconds * double(Bit32AsInteger) / MilliSecondsPerSecond);
}

uint64 CalculateTicks(uint32 seconds, uint32 milliSeconds)
{
    return static_cast<uint64>(seconds) * MicroSecondsPerSecond + static_cast<uint64>(milliSeconds) * MicroSecondsPerMilliSecond;
}

} // namespace Test
} // namespace Core
} // namespace WPEFramework
