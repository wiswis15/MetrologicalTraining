#include "Logger.h"
#include "ILogService.h"
#include "Time.h"

namespace WPEFramework {
namespace Core {

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    Logger::Logger(const string& className)
        : m_ClassName(className)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    Logger::~Logger()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    void Logger::Filter(LogLevel msgLevel, const string& message) const
    {
        uint32 timeStamp = static_cast<uint32>(Time::Now().Ticks());
        ILogService::Instance()->Log(timeStamp, msgLevel, m_ClassName, message);
    }
}
} // namespace Solution::Core
