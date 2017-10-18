// SocketPort.h: interface for the SocketPort class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SOCKETPORT_H
#define __SOCKETPORT_H

#include "Module.h"
#include "Portability.h"
#include "NodeId.h"
#include "StateTrigger.h"

#ifdef __WIN32__
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef __UNIX__
#define SOCKET signed int
#define SOCKET_ERROR static_cast<signed int>(-1)
#define INVALID_SOCKET static_cast<SOCKET>(-1)
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#endif

namespace WPEFramework {
namespace Core {
    class EXTERNAL SocketPort {
    private:
        friend class SocketMonitor;

    public:
        typedef enum {
            READ = 0x001,
            WRITE = 0x002,
            ACCEPT = 0x004,
            SHUTDOWN = 0x008,
            OPEN = 0x010,
            EXCEPTION = 0x020,
            LINK = 0x040,
            MONITOR = 0x080,
            WRITESLOT = 0x100

        } enumState;

        typedef enum {
            DATAGRAM,
            STREAM,
            LISTEN,
            RAW

        } enumType;
        // -------------------------------------------------------------------------
        // This object should not be copied, assigned or created with a default
        // constructor. Prevent them from being used, generatoed by the compiler.
        // define them but do not implement them. Compile error and/or link error.
        // -------------------------------------------------------------------------
    private:
        SocketPort(const SocketPort& a_RHS) = delete;
        SocketPort& operator=(const SocketPort& a_RHS) = delete;

    public:
        static uint32 SocketsInState(const enumState state);

#ifdef SOCKET_TEST_VECTORS
        static uint32 MonitorRuns();
#endif

    public:
        SocketPort(const enumType socketType,
            const NodeId& localNode,
            const NodeId& remoteNode,
            const uint16 sendBufferSize,
            const uint16 receiveBufferSize);

        SocketPort(const enumType socketType,
            const SOCKET& connector,
            const NodeId& remoteNode,
            const uint16 sendBufferSize,
            const uint16 receiveBufferSize);

        virtual ~SocketPort();

    public:
#ifdef __DEBUG__
        inline uint16 State() const
        {
            return (m_State);
        }
	static uint32 GetCallStack(void* buffers[], uint32 length);
#endif
        inline void RemoteNode(const NodeId& remote)
        {
            ASSERT((IsOpen() == false) || (m_SocketType == DATAGRAM));

            m_syncAdmin.Lock();
            m_RemoteNode = remote;
            m_syncAdmin.Unlock();
        }
        inline enumType Type() const
        {
            return (m_SocketType);
        }
        inline bool IsListening() const
        {
            return ((m_State & (SocketPort::SHUTDOWN | SocketPort::EXCEPTION | SocketPort::OPEN | SocketPort::LINK | SocketPort::ACCEPT)) == (SocketPort::OPEN | SocketPort::ACCEPT));
        }
        inline bool IsConnecting() const
        {
            return ((m_State & (SocketPort::SHUTDOWN | SocketPort::EXCEPTION | SocketPort::OPEN | SocketPort::LINK | SocketPort::ACCEPT)) == SocketPort::LINK);
        }
        inline bool IsSuspended() const
        {
            return ((m_State & (SocketPort::SHUTDOWN | SocketPort::EXCEPTION)) == SocketPort::SHUTDOWN);
        }
        inline bool IsForcedClosing() const
        {
            return (((m_State & (SocketPort::SHUTDOWN | SocketPort::EXCEPTION)) == (SocketPort::SHUTDOWN | SocketPort::EXCEPTION)) || ((m_State & (SocketPort::OPEN | SocketPort::EXCEPTION)) == (SocketPort::EXCEPTION)));
        }
        inline bool IsOpen() const
        {
            return ((m_State & (SocketPort::SHUTDOWN | SocketPort::EXCEPTION | SocketPort::OPEN)) == SocketPort::OPEN);
        }
        inline bool IsClosed() const
        {
            return (m_State == 0);
        }
        inline bool HasError() const
        {
            return ((m_State & (SocketPort::SHUTDOWN | SocketPort::EXCEPTION)) == SocketPort::EXCEPTION);
        }

        inline bool operator==(const SocketPort& RHS) const
        {
            return (m_Socket == RHS.m_Socket);
        }
        inline bool operator!=(const SocketPort& RHS) const
        {
            return (!operator==(RHS));
        }
        inline const NodeId& LocalNode() const
        {
            return (m_LocalNode);
        }
		inline void LocalNode(const Core::NodeId& node)
		{
			m_LocalNode = node;
		}
		inline const NodeId& RemoteNode() const
        {
            return (m_RemoteNode);
        }
		inline string LocalId() const
        {
            return (m_LocalNode.QualifiedName());
        }
        inline string RemoteId() const
        {
            return (m_RemoteNode.QualifiedName());
        }
        inline const NodeId& ReceivedNode() const
        {
            return (m_ReceivedNode);
        }
        inline uint16 SendBufferSize() const
        {
            return (m_SendBufferSize);
        }
        inline uint16 ReceiveBufferSize() const
        {
            return (m_ReceiveBufferSize);
        }
        inline void Flush()
        {
            m_syncAdmin.Lock();
            m_ReadBytes = 0;
            m_SendBytes = 0;
            m_SendOffset = 0;
            m_syncAdmin.Unlock();
        }

		uint32 TTL() const;
		uint32 TTL(const uint8 value);

        bool Join(const NodeId& multicastAddress);
        bool Leave(const NodeId& multicastAddress);
        bool Join(const NodeId& multicastAddress, const NodeId& source);
        bool Leave(const NodeId& multicastAddress, const NodeId& source);
        uint32 Open(const uint32 waitTime);
        uint32 Close(const uint32 waitTime);
        void Trigger();

        // Methods to extract and insert data into the socket buffers
        virtual uint16 SendData(uint8* dataFrame, const uint16 maxSendSize) = 0;
        virtual uint16 ReceiveData(uint8* dataFrame, const uint16 receivedSize) = 0;

        // Signal a state change, Opened, Closed or Accepted
        virtual void StateChange() = 0;

        // In case of a single connection should be accepted, these methods help
        // changing the scoket from a Listening socket to a connected socket and
        // back in case the socket closes.
        // REMARK: These methods should ONLY be called on the StateChange to
        //         Accepted (Accept) and the StateChange to Closed (Listen).
        NodeId Accept();
        void Listen();
        SOCKET Accept(NodeId& remoteId);

    private:
        inline SOCKET Socket()
        {
            return (m_Socket);
        }
        inline uint32 SocketMode() const
        {
            return (((m_SocketType == LISTEN) || (m_SocketType == STREAM)) ? SOCK_STREAM : ((m_SocketType == DATAGRAM) ? SOCK_DGRAM : SOCK_RAW));
        }
        bool Closed();
        void Opened();
        void Accepted();
        void Read();
        void Write();
        void BufferAlignment(SOCKET socket);
        SOCKET ConstructSocket(NodeId& localNode);
        uint32 WaitForOpen(const uint32 time) const;
        uint32 WaitForClosure(const uint32 time) const;

    private:
        NodeId m_LocalNode;
        NodeId m_RemoteNode;
        uint16 m_ReceiveBufferSize;
        uint16 m_SendBufferSize;
        enumType m_SocketType;
        SOCKET m_Socket;
        mutable CriticalSection m_syncAdmin;
        volatile uint16 m_State;
        NodeId m_ReceivedNode;
        uint8* m_SendBuffer;
        uint8* m_ReceiveBuffer;
        uint16 m_ReadBytes;
        uint16 m_SendBytes;
        uint16 m_SendOffset;
    };

    class EXTERNAL SocketStream : public SocketPort {
    private:
        SocketStream() = delete;
        SocketStream(const SocketStream&) = delete;
        SocketStream& operator=(const SocketStream&) = delete;

    public:
        SocketStream(const bool rawSocket,
            const NodeId& localNode,
            const NodeId& remoteNode,
            const uint16 sendBufferSize,
            const uint16 receiveBufferSize)
            : SocketPort((rawSocket ? SocketPort::RAW : SocketPort::STREAM), localNode, remoteNode, sendBufferSize, receiveBufferSize)
        {
        }

        SocketStream(const bool rawSocket,
            const SOCKET& connector,
            const NodeId& remoteNode,
            const uint16 sendBufferSize,
            const uint16 receiveBufferSize)
            : SocketPort((rawSocket ? SocketPort::RAW : SocketPort::STREAM),
                  connector, remoteNode, sendBufferSize, receiveBufferSize)
        {
        }

        virtual ~SocketStream()
        {
        }

    public:
        // Methods to extract and insert data into the socket buffers
        virtual uint16 SendData(uint8* dataFrame, const uint16 maxSendSize) = 0;
        virtual uint16 ReceiveData(uint8* dataFrame, const uint16 receivedSize) = 0;

        // Signal a state change, Opened, Closed or Accepted
        virtual void StateChange() = 0;
    };

    class EXTERNAL SocketDatagram : public SocketPort {
    private:
        SocketDatagram() = delete;
        SocketDatagram(const SocketDatagram&) = delete;
        SocketDatagram& operator=(const SocketDatagram&) = delete;

    public:
        SocketDatagram(const bool rawSocket,
            const NodeId& localNode,
            const NodeId& remoteNode,
            const uint16 sendBufferSize,
            const uint16 receiveBufferSize);
        virtual ~SocketDatagram();

    public:
        // Methods to extract and insert data into the socket buffers
        virtual uint16 SendData(uint8* dataFrame, const uint16 maxSendSize) = 0;
        virtual uint16 ReceiveData(uint8* dataFrame, const uint16 receivedSize) = 0;

        // Signal a state change, Opened, Closed or Accepted
        virtual void StateChange() = 0;
    };

    class EXTERNAL SocketListner {
    private:
        class EXTERNAL Handler : public SocketPort {
        private:
            Handler() = delete;
            Handler(const Handler&) = delete;
            Handler& operator=(const Handler&) = delete;

    public:
        Handler(SocketListner& parent) :
            SocketPort(SocketPort::LISTEN, Core::NodeId(), Core::NodeId(), 0, 0),
            _parent(parent) {
        }
        Handler(SocketListner& parent, const NodeId& refLocalNode) :
            SocketPort(SocketPort::LISTEN, refLocalNode, refLocalNode.AnyInterface(), 0, 0),
            _parent(parent) {
        }
        ~Handler() {
        }

        public:
            inline void LocalNode(const Core::NodeId& localNode)
            {
                SocketPort::LocalNode(localNode);
            }
            virtual uint16 SendData(uint8* /* dataFrame */, const uint16 /* maxSendSize */)
            {
                // This should not happen on this socket !!!!!
                ASSERT(false);

                return (0);
            }
            virtual uint16 ReceiveData(uint8* /* dataFrame */, const uint16 /* receivedSize */)
            {
                // This should not happen on this socket !!!!!
                ASSERT(false);

                return (0);
            }
            virtual void StateChange()
            {
                SOCKET newClient;
                NodeId remoteId;

                while ((newClient = SocketPort::Accept(remoteId)) != INVALID_SOCKET) {
                    _parent.Accept(newClient, remoteId);
                }
            }

        private:
            SocketListner& _parent;
        };

        // -------------------------------------------------------------------------
        // This object should not be copied, assigned or created with a default
        // constructor. Prevent them from being used, generatoed by the compiler.
        // define them but do not implement them. Compile error and/or link error.
        // -------------------------------------------------------------------------
        SocketListner(const SocketListner& a_RHS) = delete;
        SocketListner& operator=(const SocketListner& a_RHS) = delete;

    protected:
#ifdef __WIN32__
#pragma warning(disable : 4355)
#endif
        SocketListner()
            : _socket(*this)
        {
            TRACE_L5("Constructor SocketListner <0x%X>", TRACE_POINTER(this));
        }

public:
    SocketListner(const NodeId& refLocalNode) : _socket(*this, refLocalNode) {
        TRACE_L5("Constructor SocketListner <0x%X>", TRACE_POINTER(this));
    }
#ifdef __WIN32__
#pragma warning(default : 4355)
#endif

        virtual ~SocketListner()
        {
            TRACE_L5("Destructor SocketListner <0x%X>", TRACE_POINTER(this));

            _socket.Close(0);
        }

    public:
        inline bool IsListening() const
        {
            return (_socket.IsListening());
        }
        inline uint32 Open(const uint32 waitTime)
        {
            return (_socket.Open(waitTime));
        }
        inline uint32 Close(const uint32 waitTime)
        {
            return (_socket.Close(waitTime));
        }
        inline bool operator==(const SocketListner& RHS) const
        {
            return (RHS._socket == _socket);
        }
        inline bool operator!=(const SocketListner& RHS) const
        {
            return (RHS._socket != _socket);
        }

        virtual void Accept(SOCKET& newClient, const NodeId& remoteId) = 0;

    protected:
        inline void LocalNode(const Core::NodeId& localNode)
        {

            ASSERT(_socket.IsClosed() == true);

            _socket.LocalNode(localNode);
        }

    private:
        Handler _socket;
    };
}
} // namespace Core

#endif // __SOCKETPORT_H
