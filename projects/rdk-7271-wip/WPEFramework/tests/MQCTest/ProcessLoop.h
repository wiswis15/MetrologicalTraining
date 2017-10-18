#ifndef __HORIZON_PROCESSLOOP_H
#define __HORIZON_PROCESSLOOP_H

#include "../../generics/generics.h"

#include "HandleInit.h"
#include "HandleAsync.h"
#include "MonitorMessages.h"
#include "ProxyMessages.h"
#include "ProxyHandlers.h"

class ProcessLoop : public WPEFramework::Core::Thread {
public:
    ProcessLoop(WPEFramework::MQC::Postbox& channel)
        : _postOffice(channel)
        , _initHandler()
        , _asyncHandler()
    {
        if (_postOffice.IsValid() == true) {
            _postOffice.Register<WPEFramework::MQC::InitMessage>(_initHandler, 2);
            _postOffice.Register<WPEFramework::MQC::AsyncMessage>(_asyncHandler, 2);
            _postOffice.Register<WPEFramework::MQC::FloatMessage>(_floatHandler, 2);

            Run();
        }
    }
    virtual ~ProcessLoop()
    {
        Block();
        _postOffice.Quit();
        Wait(WPEFramework::Core::Thread::STOPPED | WPEFramework::Core::Thread::BLOCKED, WPEFramework::Core::infinite);

        if (_postOffice.IsValid() == true) {
            _postOffice.Unregister(_floatHandler);
            _postOffice.Unregister(_asyncHandler);
            _postOffice.Unregister(_initHandler);
        }
    }

public:
    uint32 Worker()
    {
        _postOffice.Process();
        return (WPEFramework::Core::infinite);
    }

private:
    WPEFramework::MQC::Postbox& _postOffice;
    WPEFramework::MQC::HandleInit _initHandler;
    WPEFramework::MQC::HandleAsync _asyncHandler;
    WPEFramework::MQC::HandleFloat _floatHandler;
};

#endif /* __HORIZON_PROCESSLOOP_H */
