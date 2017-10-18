#include "Logging.h"

#ifndef __WIN32__
#include <syslog.h>
#endif

namespace WPEFramework { namespace PluginHost {

/* static */ const char* MODULE_LOGGING = _T("SysLog");
static uint64 _baseTime (Core::Time::Now().Ticks()); 
static bool _syslogging (true);

void SysLog (const bool toConsole)
{
    _syslogging = !toConsole;
}

void SysLog (const char fileName[], const uint32 lineNumber, const Trace::ITrace* information)
{
    // Time to printf...
    Core::Time now (Core::Time::Now());

#ifndef __WIN32__
    if (_syslogging == true) {
        string time(now.ToRFC1123(true));
        syslog (LOG_NOTICE, "[%s]:[%s:%d]: %s: %s\n",time.c_str(), Core::FileNameOnly(fileName), lineNumber, information->Category(), information->Data());
    } else
#endif
    {
        printf ("[%11llu us] %s\n",static_cast<uint64>(now.Ticks() - _baseTime), information->Data());
    }
}

} } // namespace PluginHost
