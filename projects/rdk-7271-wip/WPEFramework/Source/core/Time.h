#ifndef __TIME_H
#define __TIME_H

#include "Portability.h"
#include "TextFragment.h"

namespace WPEFramework {
namespace Core {
    class EXTERNAL Time {
    private:


    public:
    static constexpr uint32 TicksPerMillisecond = 1000;

#ifdef __POSIX__
        Time(const struct timeval& info);
#endif
        Time(const uint16 year, const uint8 month, const uint8 day, const uint8 hour, const uint8 minute, const uint8 seconds, const uint16 milliseconds, const bool localTime);

#ifdef __POSIX__
        Time(const struct timespec& time, const bool localTime = false);
#endif
#ifdef __WIN32__
        Time(const FILETIME& time, bool localTime = false);
        inline Time(const SYSTEMTIME& time, bool localTime = false)
            : _time(time)
            , _isLocalTime(localTime)
        {
        }
#endif

        Time(const uint64 time, const bool localTime = false);

#ifdef __WIN32__
        inline Time()
        {
            _time.wYear = 0;
            _time.wMonth = 0;
            _time.wDay = 0;
            _time.wHour = 0;
            _time.wMinute = 0;
            _time.wSecond = 0;
            _time.wMilliseconds = 0;
            _time.wDayOfWeek = 0;
            _isLocalTime = false;
        }
        inline bool IsValid() const
        {
            return ((_time.wYear != 0) && (_time.wMonth != 0) && (_time.wDay != 0));
        }
        inline bool IsLocalTime() const
        {
            return (_isLocalTime == true);
        }
#endif

#ifdef __POSIX__
        inline Time()
            : _time()
            , _ticks(0)
        {
        }
        inline bool IsValid() const
        {
            return (_ticks != 0);
        }
        inline bool IsLocalTime() const
        {
            if (_time.tm_zone == nullptr)
                return false;
            uint32 value = (static_cast<uint8>(_time.tm_zone[0]) << 16) | 
                           (static_cast<uint8>(_time.tm_zone[1]) << 8) | 
                           (static_cast<uint8>(_time.tm_zone[2]) << 0);
            return (value != (('G' << 16) | ('M' << 8) | ('T'))) && (value != (('U' << 16) | ('T' << 8) | ('C')));
        }
#endif

        inline Time(const Time& copy)
            : _time(copy._time)
#ifndef __WIN32__
            , _ticks(copy._ticks)
#else
            , _isLocalTime(copy._isLocalTime)
#endif
        {
        }
        inline ~Time()
        {
        }

        inline Time& operator=(const Time& RHS)
        {
            _time = RHS._time;

#ifndef __WIN32__
            _ticks = RHS._ticks;
#else
            _isLocalTime = RHS._isLocalTime;
#endif
            return (*this);
        }

    public:
        bool FromString(const string& buffer, const bool localTime = true)
        {
            if (buffer.size() >= 18) {
                if (::isspace(buffer[3]) != 0) {
                    // Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format [18]
                    return (FromANSI(buffer, localTime));
                }
                else if (buffer[3] == ',') {
                    // Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
                    return (FromRFC1123(buffer));
                }
                else {
                    // Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
                    return (FromRFC1036(buffer));
                }
            }

            return (false);
        }

        void ToString(string& text, const bool localTime = true) const
        {
            text = ToRFC1123(localTime);
        }
        bool FromString(const string& text);
        const TCHAR * WeekDayName() const;
        const TCHAR * MonthName() const;

        inline uint32 MilliSeconds() const
        {
#ifdef __WIN32__
            return (_time.wMilliseconds);
#else
            return ((_ticks / 1000) % 1000);
#endif
        }
        inline uint8 Seconds() const
        {
#ifdef __WIN32__
            return (static_cast<uint8>(_time.wSecond));
#else
            return (static_cast<uint8>(_time.tm_sec));
#endif
        }
        inline uint8 Minutes() const
        {
#ifdef __WIN32__
            return (static_cast<uint8>(_time.wMinute));
#else
            return (static_cast<uint8>(_time.tm_min));
#endif
        }
        inline uint8 Hours() const
        {
#ifdef __WIN32__
            return (static_cast<uint8>(_time.wHour));
#else
            return (static_cast<uint8>(_time.tm_hour));
#endif
        }
        inline uint8 Day() const
        {
#ifdef __WIN32__
            return (static_cast<uint8>(_time.wDay));
#else
            return (static_cast<uint8>(_time.tm_mday));
#endif
        }
        inline uint8 Month() const
        {
#ifdef __WIN32__
            return (static_cast<uint8>(_time.wMonth));
#else
            return (static_cast<uint8>(_time.tm_mon + 1));
#endif
        }
        inline uint32 Year() const
        {
#ifdef __WIN32__
            return (static_cast<uint32>(_time.wYear));
#else
            return (static_cast<uint32>(_time.tm_year + 1900));
#endif
        }

        uint8 DayOfWeek() const;
        uint16 DayOfYear() const;
        uint64 NTPTime() const;

    sint32 DifferenceFromGMTSeconds() const;

        // Time in microseconds!
        uint64 Ticks() const;
        bool FromRFC1123(const string& buffer);
        bool FromRFC1036(const string& buffer);
        bool FromANSI(const string& buffer, const bool localTime);
        string ToRFC1123() const;
        string ToRFC1123(const bool localTime) const;

        static Time Now();
        inline static bool FromString(const string& buffer, const bool localTime, Time& element)
        {
            return (element.FromString(buffer, localTime));
        }
        inline static bool ANSI(const string& buffer, const bool localTime, Time& element)
        {
            return (element.FromANSI(buffer, localTime));
        }
        static bool RFC1123(const string& buffer, Time& element)
        {
            return (element.FromRFC1123(buffer));
        }
        static bool RFC1036(const string& buffer, Time& element)
        {
            return (element.FromRFC1036(buffer));
        }

        Time& Add(const uint32 timeInMilliseconds);
        Time& Sub(const uint32 timeInMilliseconds);

        string Format(const TCHAR* formatter) const;

        inline bool operator<(const Time& rhs) const
        {
            return (Ticks() < rhs.Ticks());
        }
        inline bool operator<=(const Time& rhs) const
        {
            return (Ticks() <= rhs.Ticks());
        }
        inline bool operator>(const Time& rhs) const
        {
            return (Ticks() > rhs.Ticks());
        }
        inline bool operator>=(const Time& rhs) const
        {
            return (Ticks() >= rhs.Ticks());
        }
        inline bool operator==(const Time& rhs) const
        {
            return (Ticks() == rhs.Ticks());
        }
        inline bool operator!=(const Time& rhs) const
        {
            return (!(operator==(rhs)));
        }
        inline Time operator-(const Time& rhs) const
        {
            return (Time(Ticks() - rhs.Ticks()));
        }
        inline Time operator+(const Time& rhs) const
        {
            return (Time(Ticks() + rhs.Ticks()));
        }
        inline Time& operator-=(const Time& rhs)
        {
            return (operator=(Time(Ticks() - rhs.Ticks())));
        }
        inline Time& operator+=(const Time& rhs)
        {
            return (operator=(Time(Ticks() + rhs.Ticks())));
        }

#ifdef __WIN32__
		inline const SYSTEMTIME& Handle() const {
			return (_time);
		}
#else
		inline const struct tm& Handle() const {
			return (_time);
		}
#endif

    private:
#ifdef __WIN32__
        mutable SYSTEMTIME _time;
        bool _isLocalTime;
#else
        struct tm _time;
        uint64 _ticks;
#endif
    };
}
} // namespace Core

#endif // __TIME_H
