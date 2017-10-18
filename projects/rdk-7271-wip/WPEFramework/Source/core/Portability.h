#ifndef __PORTABILITY_H
#define __PORTABILITY_H

#ifdef __APPLE__
#if !defined(SOL_IP)
#define SOL_IP IPPROTO_IP  /* SOL_IP is not defined on OSX Lion */
#endif /* TARGET_DARWIN && !SOL_IP */
#define CBR_110 110
#define CBR_300 300
#define CBR_600 600
#define CBR_1200 1200
#define CBR_2400 2400
#define CBR_4800 4800
#define CBR_9600 9600
#define CBR_14400 14400
#define CBR_19200 19200
#define CBR_38400 38400
#define CBR_56000 56000
#define CBR_57600 57600
#define CBR_115200 115200
#define CBR_128000 128000
#define CBR_256000 256000

#define B0 0
#define B110 110
#define B300 300
#define B600 600
#define B1200 1200
#define B2400 2400
#define B4800 4800
#define B9600 9600
#define B19200 19200
#define B38400 38400
#define B57600 57600
#define B115200 115200
#define B500000 500000
#define B1000000 1000000
#define B1152000 1152000
#define B1500000 1500000
#define B2000000 2000000
#define B2500000 2500000
#define B3000000 3000000
#define B3500000 3500000
#define B4000000 4000000
#endif

#ifdef WIN32

// W3 -- warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning(disable : 4290)

// W3 -- Make sure the "clas 'xxxx' needs to have dll-interface to be used by clients of class 'xxxx'"
#pragma warning(disable : 4251)

// W3 -- Make sure the "C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc"
#pragma warning(disable : 4530)

// W4 -- Make sure the "conditional expression is constant"
#pragma warning(disable : 4127)

// W3 -- No matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning(disable : 4291)

#ifndef __WIN32__
#define __WIN32__
#endif

#define NOMINMAX
#define _WINSOCKAPI_ /* Prevent inclusion of winsock.h in windows.h */
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
//#define NOGDI
#define NOSERVICE
#define NOMCX
#define NOIME
#include <Windows.h>
#include <TCHAR.h>
#include <string>
#include <memory.h>
#include <assert.h>
#include <algorithm>

inline void SleepS(unsigned int a_Time)
{
    ::Sleep(a_Time * 1000);
}
inline void SleepMs(unsigned int a_Time)
{
    ::Sleep(a_Time);
}
#ifdef _UNICODE
typedef std::wstring string;
#endif

#ifndef _UNICODE
typedef std::string string;
#endif

#define EXTERNAL_HIDDEN
#define EXTERNAL_EXPORT __declspec(dllexport)
#define EXTERNAL_IMPORT __declspec(dllimport)

#define CBR_110 110
#define CBR_300 300
#define CBR_600 600
#define CBR_1200 1200
#define CBR_2400 2400
#define CBR_4800 4800
#define CBR_9600 9600
#define CBR_14400 14400
#define CBR_19200 19200
#define CBR_38400 38400
#define CBR_56000 56000
#define CBR_57600 57600
#define CBR_115200 115200
#define CBR_128000 128000
#define CBR_256000 256000

#define B0 0
#define B110 CBR_110
#define B300 CBR_300
#define B600 CBR_600
#define B1200 CBR_1200
#define B2400 CBR_2400
#define B4800 CBR_4800
#define B9600 CBR_9600
#define B19200 CBR_19200
#define B38400 CBR_38400
#define B57600 CBR_57600
#define B115200 CBR_115200
#define B500000 500000
#define B1000000 1000000
#define B1152000 1152000
#define B1500000 1500000
#define B2000000 2000000
#define B2500000 2500000
#define B3000000 3000000
#define B3500000 3500000
#define B4000000 4000000

#define CS5 5
#define CS6 6
#define CS7 7
#define CS8 8

#define ALLOCA _alloca

#define KEY_LEFTSHIFT VK_LSHIFT
#define KEY_RIGHTSHIFT VK_RSHIFT
#define KEY_LEFTALT VK_LMENU
#define KEY_RIGHTALT VK_RMENU
#define KEY_LEFTCTRL VK_LCONTROL
#define KEY_RIGHTCTRL VK_RCONTROL

// This is an HTTP keyword (VERB) Let's undefine it from windows headers..
#define _CRT_SECURE_NO_WARNINGS 1
#undef DELETE

#if _MSC_VER >= 1600
const std::basic_string<char>::size_type std::basic_string<char>::npos = (std::basic_string<char>::size_type) - 1;
#endif

#define LITTLE_ENDIAN_PLATFORM 1

#else
#ifndef __LINUX__
#define __LINUX__
#endif
#endif

#ifdef __LINUX__

#include <typeinfo>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <dirent.h>
#include <fnmatch.h>
#include <dlfcn.h>
#include <algorithm>
#include <getopt.h>
#include <errno.h>
#include <alloca.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifdef __APPLE__
#include <pthread_impl.h>
#define SCHED_BATCH 99
#define MREMAP_MAYMOVE 1
#define KEY_LEFTSHIFT 2
#define KEY_RIGHTSHIFT 3
#define KEY_LEFTALT 4
#define KEY_RIGHTALT 5
#define KEY_LEFTCTRL 6
#define KEY_RIGHTCTRL 7
extern "C" void* mremap(void *old_address, size_t old_size, size_t new_size, int flags);
int clock_gettime(int, struct timespec*);
#else
#include <linux/types.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/signalfd.h>
#endif


#define ONESTOPBIT 0
#define TWOSTOPBITS CSTOPB
#define NOPARITY 0
#define EVENPARITY PARENB
#define ODDPARITY (PARENB | PARODD)

#define ESUCCESS 0
#define _Geterrno() errno
#define BYTE unsigned char
#define __POSIX__ 1
#define __UNIX__ 1

#ifdef _UNICODE
#define _T(x) L##x
#define TCHAR wchar_t
#define _tcslen wcslen
#define _tcscmp wcscmp
#define _tcsncmp wcsncmp
#define _tcsicmp wcscasecmp
#define _tcsnicmp wcsncasecmp
#define _tcschr wcschr
#define _tcsrchr wcsrchr
#define _tcsftime wcsftime
#define _stprintf swprintf
#define _tcscpy wcscpy
#define _tcsncpy wcsncpy

#define _tiscntrl iswcntrl
#define _tisprint iswprint
#define _tisspace iswspace
#define _tisblank iswblank
#define _tisgraph iswgraph
#define _tispunct iswpunct
#define _tisalnum iswalnum
#define _tisalpha iswalpha
#define _tisupper iswupper
#define _tislower iswlower
#define _tisdigit iswdigit
#define _tisxdigit iswxdigit
#endif

#ifndef _UNICODE
#define _T(x) x
#define TCHAR char
#define _tcslen strlen
#define _tcscmp strcmp
#define _tcsncmp strncmp
#define _tcsicmp strcasecmp
#define _tcsnicmp strncasecmp
#define _tcschr strchr
#define _tcsrchr strrchr
#define _tcsftime strftime
#define _stprintf sprintf
#define _tcscpy strcpy
#define _tcsncpy strncpy

#define _tiscntrl iscntrl
#define _tisprint isprint
#define _tisspace isspace
#define _tisblank isblank
#define _tisgraph isgraph
#define _tispunct ispunct
#define _tisalnum isalnum
#define _tisalpha isalpha
#define _tisupper isupper
#define _tislower islower
#define _tisdigit isdigit
#define _tisXdigit isxdigit
#endif

#define ALLOCA alloca

extern void SleepMs(unsigned int a_Time);
inline void SleepS(unsigned int a_Time)
{
    ::SleepMs(a_Time * 1000);
}

#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__((unused))
#else
#define VARIABLE_IS_NOT_USED
#endif

#ifdef __GNUC__
#define DEPRECATED __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#define DEPRECATED
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LITTLE_ENDIAN_PLATFORM 1
#elif  __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define BIG_ENDIAN_PLATFORM 1
#else
#pragma message "Unknown endianess"
#endif

#endif

#if defined(_DEBUG) || !defined(NDEBUG)
#define __DEBUG__
#endif

#ifdef __LINUX__
#if !defined(OS_ANDROID) && !defined(OS_NACL) && defined(__GLIBC__) && ( defined(__DEBUG__) || defined(CRITICAL_SECTION_LOCK_LOG) )
#include <execinfo.h>
#endif
#endif

#include "Config.h"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed char sint8;
typedef signed short sint16;
typedef signed int sint32;
typedef signed long long sint64;

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

#ifndef OK
#define OK (0)
#endif

#ifdef __WIN32__
#define SYSTEM_SYNC_HANDLE HANDLE
#else
#define SYSTEM_SYNC_HANDLE void *
#endif

#ifndef __WIN32__
#define HANDLE int
#endif

template <unsigned int TYPE>
struct TemplateIntToType {
    enum { value = TYPE };
};

extern "C" {

extern void* memrcpy(void* _Dst, const void* _Src, size_t _MaxCount);
uint64 htonll(const uint64& value);
uint64 ntohll(const uint64& value);

}

// ---- Helper types and constants ----
#define _TXT(THETEXT) _T(THETEXT) \
                      , (sizeof(THETEXT) / sizeof(TCHAR)) - 1

#define NUMBER_MAX_BITS(TYPE) (sizeof(TYPE) << 3)
#define NUMBER_MIN_UNSIGNED(TYPE) (static_cast<TYPE>(0))
#define NUMBER_MAX_UNSIGNED(TYPE) (static_cast<TYPE>(~0))
#define NUMBER_MAX_SIGNED(TYPE) (static_cast<TYPE>((((static_cast<TYPE>(1) << (NUMBER_MAX_BITS(TYPE) - 2)) - 1) << 1) + 1))
#define NUMBER_MIN_SIGNED(TYPE) (static_cast<TYPE>(-1 - NUMBER_MAX_SIGNED(TYPE)))

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    TypeName& operator=(const TypeName&);

typedef enum {
    BASE_UNKNOWN = 0,
    BASE_OCTAL = 8,
    BASE_DECIMAL = 10,
    BASE_HEXADECIMAL = 16

} NumberBase;

#ifdef __WIN32__

#include <TCHAR.h>
#define VARIABLE_IS_NOT_USED

#endif // __WIN32__

#ifdef __LINUX__

#ifdef _UNICODE
#define _T(x) L##x
#define TCHAR wchar_t
#endif

#ifndef _UNICODE
#define _T(x) x
#define TCHAR char
#endif

#if !defined(__mips__)
#define EXTERNAL_HIDDEN __attribute__((visibility("hidden")))
#else
#define EXTERNAL_HIDDEN
#endif

#endif // __LINUX__

#ifdef _UNICODE
typedef std::wstring string;
#endif

#ifndef _UNICODE
typedef std::string string;
#endif

#define STRLEN(STATIC_TEXT) ((sizeof(STATIC_TEXT) / sizeof(TCHAR)) - 1)
#define EMPTY_STRING _T("")

#ifdef __LINUX__
typedef pthread_t ThreadId;
#else
typedef HANDLE ThreadId;
#endif

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

#include "Module.h"

#if defined(__DEBUG__) || defined(CRITICAL_SECTION_LOCK_LOG)

extern "C" {

	extern void EXTERNAL DumpCallStack();
}

int EXTERNAL GetCallStack(void ** addresses, int bufferSize);
int EXTERNAL GetCallStack(ThreadId threadId, void ** addresses, int bufferSize);

#endif

#if !defined(__DEBUG)
#define DEBUG_VARIABLE(X) (void)(X)
#else
#define DEBUG_VARIABLE(x)
#endif

namespace WPEFramework {
namespace Core {
#ifdef _UNICODE
    typedef std::wstring string;
#endif

#ifndef _UNICODE
    typedef std::string string;
#endif

    inline void ToUpper(const string& input, string& output)
    {
        // Copy string to output, so we know memory is allocated.
        output = input;

        std::transform(input.begin(), input.end(), output.begin(), ::toupper);
    }

    inline void ToUpper(string& inplace)
    {
        std::transform(inplace.begin(), inplace.end(), inplace.begin(), ::toupper);
    }

    inline void ToLower(const string& input, string& output)
    {
        output = input;

        std::transform(input.begin(), input.end(), output.begin(), ::tolower);
    }

    inline void ToLower(string& inplace)
    {
        std::transform(inplace.begin(), inplace.end(), inplace.begin(), ::tolower);
    }

    const uint32 infinite = -1;
    static const string emptyString;

    class Void {
    public:
        inline Void() {}
        template <typename ARG1>
        inline Void(ARG1) {}
        inline Void(const Void&) {}
        inline ~Void() {}

        inline Void& operator=(const Void&)
        {
            return (*this);
        }
    };

	struct EXTERNAL IReferenceCounted {
		virtual ~IReferenceCounted() {};
		virtual void AddRef() const = 0;
		virtual uint32 Release() const = 0;
	};

    struct EXTERNAL IUnknown {
        enum { ID = 0x00000000 };

        virtual ~IUnknown(){};

		virtual void AddRef() const = 0;
		virtual uint32 Release() const = 0;
		virtual void* QueryInterface(const uint32 interfaceNummer) = 0;

        template <typename REQUESTEDINTERFACE>
        REQUESTEDINTERFACE* QueryInterface()
        {
            void* baseInterface(QueryInterface(REQUESTEDINTERFACE::ID));

            if (baseInterface != NULL) {
                return (reinterpret_cast<REQUESTEDINTERFACE*>(baseInterface));
            }

            return (nullptr);
        }
    };

    const uint32 ERROR_NONE = 0;
    const uint32 ERROR_GENERAL = 1;
    const uint32 ERROR_UNAVAILABLE = 2;
    const uint32 ERROR_ASYNC_FAILED = 3;
    const uint32 ERROR_ASYNC_ABORTED = 4;
    const uint32 ERROR_ILLEGAL_STATE = 5;
    const uint32 ERROR_OPENING_FAILED = 6;
    const uint32 ERROR_ACCEPT_FAILED = 7;
    const uint32 ERROR_PENDING_SHUTDOWN = 8;
    const uint32 ERROR_ALREADY_CONNECTED = 9;
    const uint32 ERROR_CONNECTION_CLOSED = 10;
    const uint32 ERROR_TIMEDOUT = 11;
    const uint32 ERROR_INPROGRESS = 12;
    const uint32 ERROR_COULD_NOT_SET_ADDRESS = 13;
    const uint32 ERROR_INCORRECT_HASH = 14;
    const uint32 ERROR_INCORRECT_URL = 15;
    const uint32 ERROR_INVALID_INPUT_LENGTH = 16;
    const uint32 ERROR_DESTRUCTION_SUCCEEDED = 17;
    const uint32 ERROR_DESTRUCTION_FAILED = 18;
    const uint32 ERROR_CLOSING_FAILED = 19;
    const uint32 ERROR_PROCESS_TERMINATED = 20;
    const uint32 ERROR_PROCESS_KILLED = 21;
    const uint32 ERROR_UNKNOWN_KEY = 22;
    const uint32 ERROR_INCOMPLETE_CONFIG = 23;
    const uint32 ERROR_PRIVILIGED_REQUEST = 24;
    const uint32 ERROR_RPC_CALL_FAILED = 25;
    const uint32 ERROR_UNREACHABLE_NETWORK = 26;
    const uint32 ERROR_REQUEST_SUBMITTED = 27;
    const uint32 ERROR_UNKNOWN_TABLE = 28;
    const uint32 ERROR_UNKNOWN_KEY_PASSED = 29;
}
}

#ifndef BUILD_REFERENCE
#define BUILD_REFERENCE engineering_build_for_debug_purpose_only
#endif

#endif // __PORTABILITY_H
