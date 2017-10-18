#ifndef __IMEDIA_H
#define __IMEDIA_H

#include "Portability.h"
#include "Module.h"

namespace WPEFramework {
namespace Core {
    struct IMedia {
        virtual ~IMedia() {}

        virtual string RemoteId() const = 0;
        virtual uint32 Open(const uint32 waitTime) = 0;
        virtual uint32 Close(const uint32 waitTime) = 0;
        virtual uint32 Reset() = 0;
        virtual uint32 Write(uint16& nSize,
            const uint8 szBytes[],
            const uint32 nTime = Core::infinite)
            = 0;
        virtual uint32 Read(uint16& nSize,
            uint8 szBytes[],
            const uint32 nTime = Core::infinite)
            = 0;
    };

    template <typename SOURCE>
    class Media : public IMedia {
    private:
        class Handler : public SOURCE {
        private:
            Handler();
            Handler(const Handler&);
            Handler& operator=(const Handler&);

        public:
            Handler(Media<SOURCE>& parent)
                : SOURCE()
                , _parent(parent)
            {
            }
            template <typename Arg1>
            Handler(Media<SOURCE>& parent, Arg1 arg1)
                : SOURCE(arg1)
                , _parent(parent)
            {
            }
            template <typename Arg1, typename Arg2>
            Handler(Media<SOURCE>& parent, Arg1 arg1, Arg2 arg2)
                : SOURCE(arg1, arg2)
                , _parent(parent)
            {
            }
            template <typename Arg1, typename Arg2, typename Arg3>
            Handler(Media<SOURCE>& parent, Arg1 arg1, Arg2 arg2, Arg3 arg3)
                : SOURCE(arg1, arg2, arg3)
                , _parent(parent)
            {
            }
            template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
            Handler(Media<SOURCE>& parent, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
                : SOURCE(arg1, arg2, arg3, arg4)
                , _parent(parent)
            {
            }
            template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
            Handler(Media<SOURCE>& parent, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
                : SOURCE(arg1, arg2, arg3, arg4, arg5)
                , _parent(parent)
            {
            }
            template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
            Handler(Media<SOURCE>& parent, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
                : SOURCE(arg1, arg2, arg3, arg4, arg5, arg6)
                , _parent(parent)
            {
            }
            ~Handler()
            {
            }

        protected:
            virtual void StateChange()
            {
                _parent.Reevaluate();
            }
            virtual uint16 SendData(uint8* dataFrame, const uint16 maxSendSize)
            {
                return (_parent.SendData(dataFrame, maxSendSize));
            }
            virtual uint16 ReceiveData(uint8* dataFrame, const uint16 receivedSize)
            {
                return (_parent.ReceiveData(dataFrame, receivedSize));
            }

        private:
            Media<SOURCE>& _parent;
        };

        Media(const Media<SOURCE>&) = delete;
        Media<SOURCE> operator=(const Media<SOURCE>&) = delete;

    public:
		#ifdef __WIN32__ 
		#pragma warning( disable : 4355 )
		#endif
        Media()
            : IMedia()
            , _channel(*this)
            , _sendSignal(false, true)
            , _sendBuffer(NULL)
            , _sendSize(0)
            , _receiveSignal(false, true)
            , _receiveBuffer(NULL)
            , _receiveSize(0)
            , _aborting(false)
            , _adminLock()
        {
        }
        template <typename Arg1>
        Media(Arg1 arg1)
            : IMedia()
            , _channel(*this, arg1)
            , _sendSignal(false, true)
            , _sendBuffer(NULL)
            , _sendSize(0)
            , _receiveSignal(false, true)
            , _receiveBuffer(NULL)
            , _receiveSize(0)
            , _aborting(false)
            , _adminLock()
        {
        }
        template <typename Arg1, typename Arg2>
        Media(Arg1 arg1, Arg2 arg2)
            : IMedia()
            , _channel(*this, arg1, arg2)
            , _sendSignal(false, true)
            , _sendBuffer(NULL)
            , _sendSize(0)
            , _receiveSignal(false, true)
            , _receiveBuffer(NULL)
            , _receiveSize(0)
            , _aborting(false)
            , _adminLock()
        {
        }
        template <typename Arg1, typename Arg2, typename Arg3>
        Media(Arg1 arg1, Arg2 arg2, Arg3 arg3)
            : IMedia()
            , _channel(*this, arg1, arg2, arg3)
            , _sendSignal(false, true)
            , _sendBuffer(NULL)
            , _sendSize(0)
            , _receiveSignal(false, true)
            , _receiveBuffer(NULL)
            , _receiveSize(0)
            , _aborting(false)
            , _adminLock()
        {
        }
        template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
        Media(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
            : IMedia()
            , _channel(*this, arg1, arg2, arg3, arg4)
            , _sendSignal(false, true)
            , _sendBuffer(NULL)
            , _sendSize(0)
            , _receiveSignal(false, true)
            , _receiveBuffer(NULL)
            , _receiveSize(0)
            , _aborting(false)
            , _adminLock()
        {
        }
        template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
        Media(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
            : IMedia()
            , _channel(*this, arg1, arg2, arg3, arg4, arg5)
            , _sendSignal(false, true)
            , _sendBuffer(NULL)
            , _sendSize(0)
            , _receiveSignal(false, true)
            , _receiveBuffer(NULL)
            , _receiveSize(0)
            , _aborting(false)
            , _adminLock()
        {
        }
        template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
        Media(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
            : IMedia()
            , _channel(*this, arg1, arg2, arg3, arg4, arg5, arg6)
            , _sendSignal(false, true)
            , _sendBuffer(NULL)
            , _sendSize(0)
            , _receiveSignal(false, true)
            , _receiveBuffer(NULL)
            , _receiveSize(0)
            , _aborting(false)
            , _adminLock()
        {
        }
		#ifdef __WIN32__ 
		#pragma warning( default : 4355 )
		#endif
		virtual ~Media()
        {
        }

    public:
        SOURCE& Source()
        {
            return (_channel);
        }
        virtual string RemoteId() const
        {
            return (_channel.RemoteId());
        }
        virtual uint32 Open(const uint32 waitTime)
        {
            return (_channel.Open(waitTime));
        }
        virtual uint32 Close(const uint32 waitTime)
        {
            return (_channel.Close(waitTime));
        }
        virtual uint32 Reset()
        {
            _adminLock.Lock();

            if ((_sendBuffer != NULL) || (_receiveBuffer != NULL)) {
                _aborting = true;

                // Make sure the blocking threads see the state change
                Reevaluate();

                _adminLock.Unlock();

                // Wait till the flag is down again..
                while (_aborting == true) {
                    // give up our slice, we wait for action..
                    SleepMs(10);
                }
            }
            else {
                _adminLock.Unlock();
            }

            _channel.Flush();

            return (OK);
        }
        virtual uint32 Write(uint16& sendSize, const uint8 dataFrame[], const uint32 duration)
        {
            uint32 result = ERROR_ILLEGAL_STATE;

            Core::Time currentTime(Core::Time::Now());
            uint32 current = static_cast<uint32>(currentTime.Ticks());

            currentTime.Add(duration);

            uint32 endTick = static_cast<uint32>(currentTime.Ticks());

            _adminLock.Lock();

            if ((_sendBuffer == NULL) && (_aborting == false)) {
                _sendSize = sendSize;
                _sendBuffer = &dataFrame[0];

                _sendSignal.ResetEvent();

                _adminLock.Unlock();

                _channel.Trigger();

                while ((_channel.IsOpen() == true) && (_sendSize != 0) && (current < endTick) && (_aborting == false)) {
                    uint32 ticksLeft = (endTick - current) / 1000;

                    TRACE_L5(_T("Waiting for %d ms"), ticksLeft);

                    _sendSignal.Lock(ticksLeft);

                    _sendSignal.ResetEvent();

                    current = static_cast<uint32>(Core::Time::Now().Ticks());
                }

                _adminLock.Lock();

                _sendBuffer = NULL;
                sendSize -= _sendSize;
                result = (_sendSize == 0 ? OK : (_aborting ? ERROR_ASYNC_ABORTED : (_channel.IsOpen() == false ? ERROR_UNAVAILABLE : ERROR_TIMEDOUT)));

                // If we were the last, triggered by the Abort, clear the Abort.
                if (_receiveBuffer == NULL) {
                    _aborting = false;
                }
            }

            _adminLock.Unlock();

            return (result);
        }
        virtual uint32 Read(uint16& receiveSize, uint8 dataFrame[], const uint32 duration)
        {
            uint32 result = ERROR_ILLEGAL_STATE;

            Core::Time currentTime(Core::Time::Now());
            uint32 current = static_cast<uint32>(currentTime.Ticks());

            currentTime.Add(duration);

            uint32 endTick = static_cast<uint32>(currentTime.Ticks());

            _adminLock.Lock();

            if ((_receiveBuffer == NULL) && (_aborting == false)) {
                _receiveSize = receiveSize;
                _receiveBuffer = &dataFrame[0];

                _adminLock.Unlock();

                _receiveSignal.ResetEvent();

                while ((_channel.IsOpen() == true) && (_receiveSize != 0) && (current < endTick) && (_aborting == false)) {
                    uint32 ticksLeft = (endTick - current) / 1000;

                    TRACE_L5(_T("Read waiting for %d ms"), ticksLeft);

                    _receiveSignal.Lock(ticksLeft);

                    _receiveSignal.ResetEvent();

                    current = static_cast<uint32>(Core::Time::Now().Ticks());
                }

                _adminLock.Lock();

                _receiveBuffer = NULL;
                receiveSize -= _receiveSize;
                result = (_receiveSize == 0 ? OK : (_aborting ? ERROR_ASYNC_ABORTED : (_channel.IsOpen() == false ? ERROR_UNAVAILABLE : ERROR_TIMEDOUT)));

                // If we were the last, triggered by the Abort, clear the Abort.
                if (_sendBuffer == NULL) {
                    _aborting = false;
                }
            }

            _adminLock.Unlock();

            return (result);
        }

    private:
        // Signal a state change, Opened, Closed or Accepted
        void Reevaluate()
        {
            // Let the process reevaluate what we did...
            _sendSignal.SetEvent();
            _receiveSignal.SetEvent();
        }
        // Methods to extract and insert data into the socket buffers
        uint16 SendData(uint8* dataFrame, const uint16 maxSendSize)
        {
            uint16 result = 0;

            _adminLock.Lock();

            if ((_sendBuffer != NULL) && (_aborting == false) && (_sendSize != 0)) {
                result = (maxSendSize > _sendSize ? _sendSize : maxSendSize);

                ASSERT(result != 0);

                // copy our buffer in, as far as we can..
                ::memcpy(dataFrame, _sendBuffer, result);

                _sendSize -= result;
                _sendBuffer = &(_sendBuffer[result]);

                _sendSignal.SetEvent();
            }

            _adminLock.Unlock();

            return (result);
        }
        uint16 ReceiveData(uint8* dataFrame, const uint16 availableData)
        {
            uint16 result = 0;

            _adminLock.Lock();

            if ((_receiveBuffer != NULL) && (_aborting == false) && (_receiveSize != 0)) {
                result = (availableData > _receiveSize ? _receiveSize : availableData);

                ASSERT(result != 0);

                // copy our buffer in, as far as we can..
                ::memcpy(_receiveBuffer, dataFrame, result);

                _receiveSize -= result;
                _receiveBuffer = &(_receiveBuffer[result]);

                _receiveSignal.SetEvent();
            }

            _adminLock.Unlock();

            return (result);
        }

    private:
        Handler _channel;

        Core::Event _sendSignal;
        const uint8* _sendBuffer;
        uint16 _sendSize;

        Core::Event _receiveSignal;
        uint8* _receiveBuffer;
        uint16 _receiveSize;

        bool _aborting;
        Core::CriticalSection _adminLock;
    };
}
} // namespace Core

#endif // __IMEDIA_H
