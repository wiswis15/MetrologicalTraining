#ifndef __SYNCHRONIZE_H
#define __SYNCHRONIZE_H

#include "Portability.h"
#include "Module.h"

namespace WPEFramework {
namespace Core {
    template <typename MESSAGE>
    class SynchronizeType {
    private:
        SynchronizeType(const SynchronizeType<MESSAGE>&);
        SynchronizeType<MESSAGE> operator=(const SynchronizeType<MESSAGE>&);

    public:
        SynchronizeType()
            : _response(NULL)
            , _signal(false, true)
        {
        }
        virtual ~SynchronizeType()
        {
        }

    public:
        template <typename INBOUND>
        bool Evaluate(const INBOUND& newMessage)
        {
            bool result = false;

            _adminLock.Lock();

            if ((_response != NULL) && (_response->Copy(newMessage) == true)) {
                // It is the message we where waiting on. Signal succesfull retrieval.
                result = true;
                _response = NULL;
                _signal.SetEvent();
            }

            _adminLock.Unlock();

            return (result);
        }
        inline void Flush()
        {
            _signal.SetEvent();
        }
        void Load(MESSAGE& response)
        {
            _adminLock.Lock();

            ASSERT(_response == NULL);

            _signal.ResetEvent();

            _response = &response;

            _adminLock.Unlock();
        }
        uint32 Aquire(const uint32 duration)
        {
            uint32 result = Core::ERROR_NONE;

            Core::Time currentTime(Core::Time::Now());

            currentTime.Add(duration);

            uint32 endTick = static_cast<uint32>(currentTime.Ticks());

            _signal.Lock(endTick);

            _adminLock.Lock();

            if (_response != NULL) {
                // Seems it failed, question why?
                result = (_signal.IsSet() ? Core::ERROR_ASYNC_ABORTED : Core::ERROR_TIMEDOUT);
                _response = NULL;
            }

            _adminLock.Unlock();

            return (result);
        }

    private:
        MESSAGE* _response;
        Core::Event _signal;
        Core::CriticalSection _adminLock;
    };
}
} // namespace Core

#endif // __SYNCHRONIZE_H
