#include <gtest/gtest.h>

#include <generics/Time.h>
#include "TimeHelpers.h"

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

class TimeTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        // In order to be independent of the actual local time, we need to know how far off GMT we are.
        Time tester(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);

        DiffFromGMT = tester.DifferenceFromGMTSeconds();
        IsLocalTimeDifferentFromGMT = DiffFromGMT != 0;
    }

    virtual void TearDown() {}

#ifdef __WIN32__
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
#endif // __WIN32__

#ifdef __POSIX__

    // Calculate epoch time (seconds since 1970-01-01 00:00:00) for local time
    time_t EpochTime(const Time &time)
    {
        struct tm timeLocal = time.Handle();
        return mktime(&timeLocal);
    }

    // Calculate epoch time (seconds since 1970-01-01 00:00:00) for GMT time
    time_t EpochTimeGMT(const Time &time)
    {
        struct tm timeLocal = time.Handle();
        return mktimegm(&timeLocal);
    }

#endif // __POSIX__

    // Convert a local time to a GMT time string (RFC1123)
    string ConvertLocalToGMT(const Time &time)
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
    string ConvertGMTToLocal(const Time &time)
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

    void ExpectTime(const Time &target, uint32 year, uint8 month, uint8 day, uint8 hour, uint8 minute, uint8 second,
                    uint32 milliSeconds, uint8 dayOfWeek)
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

TEST_F(TimeTest, ConstructCopy)
{
    Time other(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);
    Time target(other);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, Assignment)
{
    Time other(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);
    Time target;
    target = other;

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, ConstructDateTimeLocal)
{
    Time target(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, ConstructDateTimeGMT)
{
    Time target(Year, Month, Day, Hour, Minute, Second, MilliSeconds, false);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, ConstructTicksLocal)
{
    uint64 ticks = CalculateTicks(EpochTimestampGMT - DiffFromGMT, MilliSeconds);
    Time target(ticks, true);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT - DiffFromGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, ConstructTicksGMT)
{
    uint64 ticks = CalculateTicks(EpochTimestampGMT, MilliSeconds);
    Time target(ticks, false);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, Year, Month, Day, Hour, Minute, Second, MilliSeconds, Friday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampGMT, MilliSeconds), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampGMT, MilliSeconds), target.Ticks());
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromStringLocalANSI)
{
    char inputANSI[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    target.FromString(inputANSI, true);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromStringLocalRFC1123)
{
    char inputRFC1123[] = "Sun, 06 Nov 1994 08:49:37";

    Time target;
    target.FromString(inputRFC1123, true);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromStringLocalRFC1036)
{
    char inputRFC1036[] = "Sunday, 06-Nov-94 08:49:37";

    Time target;
    target.FromString(inputRFC1036, true);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromStringGMT_ANSI)
{
    char inputANSI[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    target.FromString(inputANSI, false);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromStringGMT_RFC1123)
{
    char inputRFC1123[] = "Sun, 06 Nov 1994 08:49:37 GMT";

    Time target;
    target.FromString(inputRFC1123, false);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromStringGMT_RFC1036)
{
    char inputRFC1036[] = "Sunday, 06-Nov-94 08:49:37 GMT";

    Time target;
    target.FromString(inputRFC1036, false);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromANSILocal)
{
    char input[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    target.FromANSI(input, true);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromANSIGMT)
{
    char input[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    target.FromANSI(input, false);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromRFC1123Local)
{
    char input[] = "Sun, 06 Nov 1994 08:49:37";

    Time target;
    target.FromRFC1123(input);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromRFC1123GMT)
{
    char input[] = "Sun, 06 Nov 1994 08:49:37 GMT";

    Time target;
    target.FromRFC1123(input);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromRFC1036Local)
{
    char input[] = "Sunday, 06-Nov-94 08:49:37";

    Time target;
    target.FromRFC1036(input);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromRFC1036GMT)
{
    char input[] = "Sunday, 06-Nov-94 08:49:37 GMT";

    Time target;
    target.FromRFC1036(input);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, ToString)
{
    Time target(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);
    string resultLocal;
    target.ToString(resultLocal, true);

    string resultGMT;
    target.ToString(resultGMT, false);

    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", resultLocal);
    EXPECT_EQ(ConvertLocalToGMT(target), resultGMT);
}

TEST_F(TimeTest, ToRFC1123)
{
    Time targetLocal(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);
    Time targetGMT(Year, Month, Day, Hour, Minute, Second, MilliSeconds, false);

    if (IsLocalTimeDifferentFromGMT)
        EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", targetLocal.ToRFC1123());
    else
        EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 GMT", targetLocal.ToRFC1123());
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 GMT", targetGMT.ToRFC1123());
}

TEST_F(TimeTest, ToRFC1123Local)
{
    Time target(Year, Month, Day, Hour, Minute, Second, MilliSeconds, true);

    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 ", target.ToRFC1123(true));
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
}

TEST_F(TimeTest, ToRFC1123GMT)
{
    Time target(Year, Month, Day, Hour, Minute, Second, MilliSeconds, false);

    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_EQ("Fri, 17 Feb 2017 12:53:16 GMT", target.ToRFC1123(false));
}

TEST_F(TimeTest, FromStringStaticLocalANSI)
{
    char inputANSI[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    Time::FromString(inputANSI, true, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromStringStaticLocalRFC1123)
{
    char inputRFC1123[] = "Sun, 06 Nov 1994 08:49:37";

    Time target;
    Time::FromString(inputRFC1123, true, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromStringStaticLocalRFC1036)
{
    char inputRFC1036[] = "Sunday, 06-Nov-94 08:49:37";

    Time target;
    Time::FromString(inputRFC1036, true, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, FromStringStaticGMT_ANSI)
{
    char inputANSI[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    Time::FromString(inputANSI, false, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromStringStaticGMT_RFC1123)
{
    char inputRFC1123[] = "Sun, 06 Nov 1994 08:49:37 GMT";

    Time target;
    Time::FromString(inputRFC1123, false, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, FromStringStaticGMT_RFC1036)
{
    char inputRFC1036[] = "Sunday, 06-Nov-94 08:49:37 GMT";

    Time target;
    Time::FromString(inputRFC1036, false, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, ANSILocal)
{
    char input[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    Time::ANSI(input, true, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, ANSIGMT)
{
    char input[] = "Sun Nov  6 08:49:37 1994";

    Time target;
    Time::ANSI(input, false, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, RFC1123Local)
{
    char input[] = "Sun, 06 Nov 1994 08:49:37";

    Time target;
    Time::RFC1123(input, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, RFC1123GMT)
{
    char input[] = "Sun, 06 Nov 1994 08:49:37 GMT";

    Time target;
    Time::RFC1123(input, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, RFC1036Local)
{
    char input[] = "Sunday, 06-Nov-94 08:49:37";

    Time target;
    Time::RFC1036(input, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT - DiffFromGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT - DiffFromGMT, 0), target.Ticks());
    EXPECT_EQ(ConvertLocalToGMT(target), target.ToRFC1123(false));
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 ", target.ToRFC1123(true));
    EXPECT_EQ(IsLocalTimeDifferentFromGMT, target.IsLocalTime());
}

TEST_F(TimeTest, RFC1036GMT)
{
    char input[] = "Sunday, 06-Nov-94 08:49:37 GMT";

    Time target;
    Time::RFC1036(input, target);

    EXPECT_TRUE(target.IsValid());
    ExpectTime(target, YearANSI, MonthANSI, DayANSI, HourANSI, MinuteANSI, SecondANSI, 0, Sunday);
    EXPECT_EQ(CalculateNTPTime(EpochTimestampANSIGMT, 0), target.NTPTime());
    EXPECT_EQ(CalculateTicks(EpochTimestampANSIGMT, 0), target.Ticks());
    EXPECT_EQ("Sun, 06 Nov 1994 08:49:37 GMT", target.ToRFC1123(false));
    EXPECT_EQ(ConvertGMTToLocal(target), target.ToRFC1123(true));
    EXPECT_FALSE(target.IsLocalTime());
}

TEST_F(TimeTest, Add)
{
    const uint32 SubSecond = 333;
    Time target(Year, Month, Day, Hour, Minute, Second, SubSecond, true);
    target.Add(SubSecond);

    EXPECT_EQ(2 * SubSecond, target.MilliSeconds());
    EXPECT_EQ(Second, target.Seconds());

    target.Add(SubSecond * 2);

    EXPECT_EQ(4 * SubSecond - MilliSecondsPerSecond, target.MilliSeconds());
    EXPECT_EQ(Second + 1, target.Seconds());
}

TEST_F(TimeTest, AddOneDay)
{
    const uint32 Increment = SecondsPerDay * MilliSecondsPerSecond;
    Time target(Year, Month, Day, Hour, Minute, Second, 0, true);
    target.Add(Increment);

    EXPECT_EQ(Year, target.Year());
    EXPECT_EQ(Month, target.Month());
    EXPECT_EQ(Day + 1, target.Day());
    EXPECT_EQ(Hour, target.Hours());
    EXPECT_EQ(Minute, target.Minutes());
    EXPECT_EQ(Second, target.Seconds());
}

TEST_F(TimeTest, Sub)
{
    const uint32 SubSecond = 333;

    Time target(Year, Month, Day, Hour, Minute, Second, SubSecond, true);
    target.Sub(SubSecond);

    EXPECT_EQ(0, target.MilliSeconds());
    EXPECT_EQ(Second, target.Seconds());

    target.Sub(SubSecond);

    EXPECT_EQ(MilliSecondsPerSecond - SubSecond, target.MilliSeconds());
    EXPECT_EQ(Second - 1, target.Seconds());
}

TEST_F(TimeTest, SubOneDay)
{
    const uint32 Decrement = SecondsPerDay * MilliSecondsPerSecond;
    Time target(Year, Month, Day, Hour, Minute, Second, 0, true);
    target.Sub(Decrement);

    EXPECT_EQ(Year, target.Year());
    EXPECT_EQ(Month, target.Month());
    EXPECT_EQ(Day - 1, target.Day());
    EXPECT_EQ(Hour, target.Hours());
    EXPECT_EQ(Minute, target.Minutes());
    EXPECT_EQ(Second, target.Seconds());
}

TEST_F(TimeTest, ComparisonSmaller)
{
    Time ref1(2017, 1, 1, 23, 59, 59, 0, false);
    Time ref2(2017, 1, 1, 23, 59, 58, 0, false);
    Time ref3(2017, 1, 2, 0, 0, 0, 0, false);

    EXPECT_FALSE(ref1 < ref1);
    EXPECT_FALSE(ref1 < ref2);
    EXPECT_TRUE(ref1 < ref3);
    EXPECT_TRUE(ref2 < ref1);
    EXPECT_FALSE(ref2 < ref2);
    EXPECT_TRUE(ref2 < ref3);
    EXPECT_FALSE(ref3 < ref1);
    EXPECT_FALSE(ref3 < ref2);
    EXPECT_FALSE(ref3 < ref3);
}

TEST_F(TimeTest, ComparisonSmallerEqual)
{
    Time ref1(2017, 1, 1, 23, 59, 59, 0, false);
    Time ref2(2017, 1, 1, 23, 59, 58, 0, false);
    Time ref3(2017, 1, 2, 0, 0, 0, 0, false);

    EXPECT_TRUE(ref1 <= ref1);
    EXPECT_FALSE(ref1 <= ref2);
    EXPECT_TRUE(ref1 <= ref3);
    EXPECT_TRUE(ref2 <= ref1);
    EXPECT_TRUE(ref2 <= ref2);
    EXPECT_TRUE(ref2 <= ref3);
    EXPECT_FALSE(ref3 <= ref1);
    EXPECT_FALSE(ref3 <= ref2);
    EXPECT_TRUE(ref3 <= ref3);
}

TEST_F(TimeTest, ComparisonGreater)
{
    Time ref1(2017, 1, 1, 23, 59, 59, 0, false);
    Time ref2(2017, 1, 1, 23, 59, 58, 0, false);
    Time ref3(2017, 1, 2, 0, 0, 0, 0, false);

    EXPECT_FALSE(ref1 > ref1);
    EXPECT_TRUE(ref1 > ref2);
    EXPECT_FALSE(ref1 > ref3);
    EXPECT_FALSE(ref2 > ref1);
    EXPECT_FALSE(ref2 > ref2);
    EXPECT_FALSE(ref2 > ref3);
    EXPECT_TRUE(ref3 > ref1);
    EXPECT_TRUE(ref3 > ref2);
    EXPECT_FALSE(ref3 > ref3);
}

TEST_F(TimeTest, ComparisonGreaterEqual)
{
    Time ref1(2017, 1, 1, 23, 59, 59, 0, false);
    Time ref2(2017, 1, 1, 23, 59, 58, 0, false);
    Time ref3(2017, 1, 2, 0, 0, 0, 0, false);

    EXPECT_TRUE(ref1 >= ref1);
    EXPECT_TRUE(ref1 >= ref2);
    EXPECT_FALSE(ref1 >= ref3);
    EXPECT_FALSE(ref2 >= ref1);
    EXPECT_TRUE(ref2 >= ref2);
    EXPECT_FALSE(ref2 >= ref3);
    EXPECT_TRUE(ref3 >= ref1);
    EXPECT_TRUE(ref3 >= ref2);
    EXPECT_TRUE(ref3 >= ref3);
}

TEST_F(TimeTest, ComparisonEqual)
{
    Time ref1(2017, 1, 1, 23, 59, 59, 0, false);
    Time ref2(2017, 1, 1, 23, 59, 58, 0, false);
    Time ref3(2017, 1, 2, 0, 0, 0, 0, false);

    EXPECT_TRUE(ref1 == ref1);
    EXPECT_FALSE(ref1 == ref2);
    EXPECT_FALSE(ref1 == ref3);
    EXPECT_FALSE(ref2 == ref1);
    EXPECT_TRUE(ref2 == ref2);
    EXPECT_FALSE(ref2 == ref3);
    EXPECT_FALSE(ref3 == ref1);
    EXPECT_FALSE(ref3 == ref2);
    EXPECT_TRUE(ref3 == ref3);
}

TEST_F(TimeTest, ComparisonNotEqual)
{
    Time ref1(2017, 1, 1, 23, 59, 59, 0, false);
    Time ref2(2017, 1, 1, 23, 59, 58, 0, false);
    Time ref3(2017, 1, 2, 0, 0, 0, 0, false);

    EXPECT_FALSE(ref1 != ref1);
    EXPECT_TRUE(ref1 != ref2);
    EXPECT_TRUE(ref1 != ref3);
    EXPECT_TRUE(ref2 != ref1);
    EXPECT_FALSE(ref2 != ref2);
    EXPECT_TRUE(ref2 != ref3);
    EXPECT_TRUE(ref3 != ref1);
    EXPECT_TRUE(ref3 != ref2);
    EXPECT_FALSE(ref3 != ref3);
}

TEST_F(TimeTest, OperationAdd)
{
    Time original(2017, 1, 1, 23, 59, 59, 0, false);
    Time argument(1000, false);
    Time result = original + argument;

    EXPECT_EQ(original.Ticks() + 1000, result.Ticks());
}

TEST_F(TimeTest, OperationSubtract)
{
    Time original(2017, 1, 1, 23, 59, 59, 0, false);
    Time argument(2017, 1, 1, 23, 59, 58, 0, false);
    Time result = original - argument;

    EXPECT_EQ(MicroSecondsPerSecond, result.Ticks());
}

TEST_F(TimeTest, OperationAddTo)
{
    Time original(2017, 1, 1, 23, 59, 59, 0, false);
    Time argument(1000, false);
    Time result = original;
    result += argument;

    EXPECT_EQ(original.Ticks() + 1000, result.Ticks());
}

TEST_F(TimeTest, OperationSubtractFrom)
{
    Time original(2017, 1, 1, 23, 59, 59, 0, false);
    Time argument(2017, 1, 1, 23, 59, 58, 0, false);
    Time result = original;
    result -= argument;

    EXPECT_EQ(MicroSecondsPerSecond, result.Ticks());
}

static constexpr double TimeInterval1Second = MicroSecondsPerSecond;
static constexpr double TimeJitter10MilliSecond = 10 * MilliSecondsPerSecond;

TEST_F(TimeTest, Now)
{
    Time before = Time::Now();
    SleepS(1);
    Time now = Time::Now();

    EXPECT_TRUE(now > before);
    Time diff = now - before;
    // We expect less than 10 ms jitter
    EXPECT_NEAR(static_cast<double>(diff.Ticks()), TimeInterval1Second, TimeJitter10MilliSecond);
}

} // namespace Test
} // namespace Core
} // namespace WPEFramework
