#ifndef __TRACEMEDIA_H
#define __TRACEMEDIA_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "Module.h"
#include "ITraceMedia.h"

// ---- Referenced classes and types ----

// ---- Helper functions ----

// ---- Class Definition ----
namespace WPEFramework {
namespace Trace {
    // ---- Helper types and constants ----
    struct ITrace;

    class EXTERNAL TraceMedia : public ITraceMedia {
    private:
        class Channel : public Core::SocketDatagram {
        public:
            Channel(TraceMedia& parent, const Core::NodeId& remoteNode)
                : Core::SocketDatagram(false, remoteNode.Origin(), remoteNode, 2048, TRACINGBUFFERSIZE + 512)
                , _loaded(0)
                , // 32 bytes for preamble
                _parent(parent)
            {
            }
            virtual ~Channel()
            {
                Close(Core::infinite);
            }

        public:
            // Methods to extract and insert data into the socket buffers
            virtual uint16 SendData(uint8* dataFrame, const uint16 maxSendSize)
            {
                // We used it all up..
                // TODO: make thread safe.

                uint16 actualByteCount = _loaded > maxSendSize ? maxSendSize : _loaded;
                memcpy(dataFrame, _traceBuffer, actualByteCount);
                _loaded = 0;
                
                return (actualByteCount);
            }
            virtual uint16 ReceiveData(uint8* dataFrame, const uint16 receivedSize)
            {
                _parent.HandleMessage(dataFrame, receivedSize);

                return (receivedSize);
            }
            // Signal a state change, Opened, Closed or Accepted
            virtual void StateChange()
            {
            }

            // TODO: lock to keep this consistent
            char _traceBuffer[1500];
            uint16 _loaded;
            
            // TODO: create message here, not in parent

        private:
        
            TraceMedia& _parent;
        };

    private:
        TraceMedia();
        TraceMedia(const TraceMedia&);
        TraceMedia& operator=(const TraceMedia&);

    public:
        TraceMedia(const Core::NodeId& nodeId);
        ~TraceMedia();

        virtual void Output(
            const char fileName[],
            const uint32 lineNumber,
            const char className[],
            const ITrace* information);

    private:
        char* CopyText(char* destination, const char* source, uint32 maxSize);
        void HandleMessage(const uint8* dataFrame, const uint16 receivedSize);
        void Write(const uint8* dataFrame, const uint16 receivedSize);

    private:
        Channel m_Output;
    };
}
} // namespace Trace

#endif // __TRACEMEDIA_H
