#include <gtest/gtest.h>

#include <generics/Time.h>
#include "TimeHelpers.h"

#ifdef __WIN32__

namespace WPEFramework {
namespace Core {
namespace Test {

static constexpr uint32 Year = 2017;
static constexpr uint8 Month = 2;
static constexpr uint8 Day = 17;
static constexpr uint8 Hour = 12;
static constexpr uint8 Minute = 53;
static constexpr uint8 Second = 16;
static constexpr uint32 MilliSeconds = 234;
static constexpr uint8 Sunday = 0; // Day of week
static constexpr uint8 Friday = 5; // Day of week
static constexpr uint8 MonthOffset = 1;
static constexpr uint32 YearOffset = 1900;
static constexpr uint64 NTPTime = 6398434384988826763;
static constexpr uint32 EpochTimestampGMT = 1487335996;

static constexpr uint32 MicroSeconds = MilliSeconds * MicroSecondsPerMilliSecond;
static constexpr uint32 NanoSeconds = MilliSeconds * NanoSecondsPerMilliSecond;

static constexpr uint32 YearANSI = 1994;
static constexpr uint8 MonthANSI = 11;
static constexpr uint8 DayANSI = 6;
static constexpr uint8 HourANSI = 8;
static constexpr uint8 MinuteANSI = 49;
static constexpr uint8 SecondANSI = 37;
static constexpr uint32 EpochTimestampANSIGMT = 784111777;

class TimeTestWindows : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        // In order to be independent of the actual local time, we need to know how far off GMT we are.
        Time tester(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);

        DiffFromGMT = tester.DifferenceFromGMTSeconds();
        IsLocalTimeDifferentFromGMT = DiffFromGMT != 0;
    }

    virtual void TearDown() {}

    void SystemTimeToStructTM(const SYSTEMTIME & st, struct tm & tm)
    {
        tm.tm_isdst = -1; // mktime() computes whether this is during Standard or Daylight time. */
        tm.tm_sec = static_cast<int>(st.wSecond);
        tm.tm_min = static_cast<int>(st.wMinute);
        tm.tm_hour = static_cast<int>(st.wHour);
        tm.tm_mday = static_cast<int>(st.wDay);
        tm.tm_mon = static_cast<int>(st.wMonth - 1);
        tm.tm_year = static_cast<int>(st.wYear - 1900);
        tm.tm_wday = static_cast<int>(st.wDayOfWeek);
    }
    
    // Calculate epoch time (seconds since 1970-01-01 00:00:00) for local time
    time_t EpochTime(const Time & time)
    {
        struct tm timeLocal;
        SystemTimeToStructTM(time.Handle(), timeLocal);
        return mktime(&timeLocal);
    }
    // Calculate epoch time (seconds since 1970-01-01 00:00:00) for GMT time
    time_t EpochTimeGMT(const Time & time)
    {
        struct tm timeLocal;
        SystemTimeToStructTM(time.Handle(), timeLocal);
        return mktimegm(&timeLocal);
    }

    // Convert a local time to a GMT time string (RFC1123)
    string ConvertLocalToGMT(const Time & time)
    {
        time_t timestamp = EpochTime(time);

        struct tm timeGMT = *gmtime(&timestamp);
        std::ostringstream stream;
        stream << WeekDayName(timeGMT.tm_wday) << ", "
               << std::setw(2) << std::setfill('0') << timeGMT.tm_mday << " "
               << MonthName(timeGMT.tm_mon) << " "
               << timeGMT.tm_year + 1900 << " "
               << std::setw(2) << std::setfill('0') << timeGMT.tm_hour << ":"
               << std::setw(2) << std::setfill('0') << timeGMT.tm_min << ":"
               << std::setw(2) << std::setfill('0') << timeGMT.tm_sec << " GMT";
        return stream.str();
    }
    // Convert a GMT time to a local time string (RFC1123)
    string ConvertGMTToLocal(const Time & time)
    {
        time_t timestamp = EpochTimeGMT(time);

        struct tm timeLocal = *localtime(&timestamp);
        std::ostringstream stream;
        stream << WeekDayName(timeLocal.tm_wday) << ", "
               << std::setw(2) << std::setfill('0') << timeLocal.tm_mday << " "
               << MonthName(timeLocal.tm_mon) << " "
               << timeLocal.tm_year + 1900 << " "
               << std::setw(2) << std::setfill('0') << timeLocal.tm_hour << ":"
               << std::setw(2) << std::setfill('0') << timeLocal.tm_min << ":"
               << std::setw(2) << std::setfill('0') << timeLocal.tm_sec << " ";
        return stream.str();
    }

    void ExpectTime(const Time & target, uint32 year, uint8 month, uint8 day, uint8 hour, uint8 minute, uint8 second, uint32 milliSeconds, uint8 dayOfWeek)
    {
        EXPECT_EQ(milliSeconds, target.MilliSeconds());
        EXPECT_EQ(second, target.Seconds());
        EXPECT_EQ(minute, target.Minutes());
        EXPECT_EQ(hour, target.Hours());
        EXPECT_EQ(day, target.Day());
        EXPECT_EQ(month, target.Month());
        EXPECT_EQ(year, target.Year());
        EXPECT_EQ(dayOfWeek, target.DayOfWeek());
    }

protected:
    sint32 DiffFromGMT;
    bool IsLocalTimeDifferentFromGMT;
};

TEST_F(TimeTestWindows, Construct)
{
    Time target;
    EXPECT_FALSE(target.IsValid());
    ExpectTime(target, 0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(CalculateNTPTime(0, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(0, 0), target.Ticks());
    EXPECT_EQ("", target.ToRFC1123(false));
    EXPECT_EQ("", target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTestWindows, ConstructFileTimeLocal)
{
    SYSTEMTIME systemTime = 
    {
        Year,
        Month,
        Friday,
        Day,
        Hour,
        Minute,
        Second,
        MilliSeconds
    };
    FILETIME fileTime {};
    SystemTimeToFileTime(&systemTime, &fileTime);
    Time target(fileTime, true);
    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTestWindows, ConstructFileTimeGMT)
{
    SYSTEMTIME systemTime =
    {
        Year,
        Month,
        Friday,
        Day,
        Hour,
        Minute,
        Second,
        MilliSeconds
    };
    FILETIME fileTime{};
    SystemTimeToFileTime(&systemTime, &fileTime);
    Time target(fileTime, false);
    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_EQ(false, target.IsLocalTime());
}

TEST_F(TimeTestWindows, FormatLocal)
{
    Time target(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);
    string actual;
    string expected;

    actual = target.Format("%a %A");
    expected = "Fri Friday";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%b %B");
    expected = "Feb February";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%c");
    expected = "Fri Feb 17 12:53:16 2017";

    EXPECT_EQ(expected, actual);

	// %C is not officially supported
    actual = target.Format("%C");
    expected = "20";

    EXPECT_EQ(expected, actual);

	// %D is not officially supported
	actual = target.Format("%d %D");
    expected = "17 02/17/17";

    EXPECT_EQ(expected, actual);

	// %e is not officially supported
	actual = target.Format("%e");
    expected = "17";

    EXPECT_EQ(expected, actual);

	// %F is not officially supported
	actual = target.Format("%F");
    expected = "2017-02-17";

    EXPECT_EQ(expected, actual);

	// %g, %G is not officially supported, and incorrect
    //actual = target.Format("%g %G");
    //expected = "17 2017";

    //EXPECT_EQ(expected, actual);

    // %h is not officially supported
    actual = target.Format("%h %H %I");
    expected = "Feb 12 12";

    EXPECT_EQ(expected, actual);

    // %k, %l is not supported
    //actual = target.Format("%j %k %l");
    actual = target.Format("%j");
    //expected = "048 12 12";
    expected = "048";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%m %M");
    expected = "02 53";

    EXPECT_EQ(expected, actual);

    // %P is not supported
    //actual = target.Format("%p %P");
    actual = target.Format("%p");
    //expected = "PM pm";
    expected = "PM";

    EXPECT_EQ(expected, actual);

    // %r, %R is not officially supported
    actual = target.Format("%r %R");
    expected = "12:53:16 PM 12:53";

    EXPECT_EQ(expected, actual);

    // %s is not supported
    //actual = target.Format("%s");
    //std::ostringstream stream;
    //stream << EpochTime(target);
    //expected = stream.str();

    //EXPECT_EQ(expected, actual);

    actual = target.Format("%S");
    expected = "16";

    EXPECT_EQ(expected, actual);

    // %T, %u is not officially supported
    actual = target.Format("%T %u %U");
    expected = "12:53:16 5 07";

    EXPECT_EQ(expected, actual);

    // %V is not officially supported
    actual = target.Format("%V %w %W");
    expected = "07 5 07";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%x %X %y %Y");
    expected = "02/17/17 12:53:16 17 2017";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%z");
    std::ostringstream stream;
    stream.str("");
    stream << ((DiffFromGMT >= 0) ? "+" : "-")
           << std::setw(2) << std::setfill('0') << (DiffFromGMT / 3600)
           << std::setw(2) << std::setfill('0') << ((DiffFromGMT % 3600) / 60);
    expected = stream.str();

    EXPECT_EQ(expected, actual);
}

TEST_F(TimeTestWindows, FormatGMT)
{
    Time target(2017, 1, 1, 23, 59, 58, 0, false);
    string actual;
    string expected;

    actual = target.Format("%a %A");
    expected = "Sun Sunday";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%b %B");
    expected = "Jan January";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%c");
    expected = "Sun Jan  1 23:59:58 2017";

    EXPECT_EQ(expected, actual);

    // %C is not officially supported
    actual = target.Format("%C");
    expected = "20";

    EXPECT_EQ(expected, actual);

    // %D is not officially supported
    actual = target.Format("%d %D");
    expected = "01 01/01/17";

    EXPECT_EQ(expected, actual);

    // %e is not officially supported
    actual = target.Format("%e");
    expected = " 1";

    EXPECT_EQ(expected, actual);

    // %F is not officially supported
    actual = target.Format("%F");
    expected = "2017-01-01";

    EXPECT_EQ(expected, actual);

    // %g, %G is not officially supported, and incorrect
    //actual = target.Format("%g %G");
    //expected = "16 2016";

    //EXPECT_EQ(expected, actual);

    // %h is not officially supported
    actual = target.Format("%h %H %I");
    expected = "Jan 23 11";

    EXPECT_EQ(expected, actual);

    // %k, %l is not supported
    //actual = target.Format("%j %k %l");
    actual = target.Format("%j");
    //expected = "001 23 1";
    expected = "001";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%m %M");
    expected = "01 59";

    EXPECT_EQ(expected, actual);

    // %P is not supported
    //actual = target.Format("%p %P");
    actual = target.Format("%p");
    //expected = "PM pm";
    expected = "PM";

    EXPECT_EQ(expected, actual);

    // %r, %R is not officially supported
    actual = target.Format("%r %R");
    expected = "11:59:58 PM 23:59";

    EXPECT_EQ(expected, actual);

    // %s is not supported
    //actual = target.Format("%s");
    //std::ostringstream stream;
    //stream << EpochTime(target);
    //expected = stream.str();

    //EXPECT_EQ(expected, actual);

    actual = target.Format("%S");
    expected = "58";

    EXPECT_EQ(expected, actual);

    // %T, %u is not officially supported
    actual = target.Format("%T %u %U");
    expected = "23:59:58 7 01";

    EXPECT_EQ(expected, actual);

    // %V is not officially supported
    actual = target.Format("%V %w %W");
    expected = "52 0 00";

    EXPECT_EQ(expected, actual);

    actual = target.Format("%x %X %y %Y");
    expected = "01/01/17 23:59:58 17 2017";

    EXPECT_EQ(expected, actual);

    // Don't check %Z as value depends on registry setting
    // %z behaves differently, it always specifies the local time zone
    //actual = target.Format("%z %Z");
    actual = target.Format("%z");
    //expected = "+0000 GMT";
    std::ostringstream stream;
    stream.str("");
    stream << ((DiffFromGMT >= 0) ? "+" : "-")
        << std::setw(2) << std::setfill('0') << (DiffFromGMT / 3600)
        << std::setw(2) << std::setfill('0') << ((DiffFromGMT % 3600) / 60);
    expected = stream.str();

    EXPECT_EQ(expected, actual);
}

} // namespace Test
} // namespace Core
} // namespace WPEFramework

#endif // __WIN32__