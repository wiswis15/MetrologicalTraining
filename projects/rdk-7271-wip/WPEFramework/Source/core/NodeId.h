// NodeId.h: interface for the NodeId class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NODEID_H
#define __NODEID_H

#include "Module.h"

#ifdef __WIN32__
#include <winsock2.h>
#include <Ws2ipdef.h>
#pragma comment(lib, "wsock32.lib")
#endif

#ifdef __UNIX__
#include <netinet/in.h>
#include <sys/un.h>
#endif

#include "TextFragment.h"

namespace WPEFramework {
namespace Core {
class EXTERNAL NodeId {
public:
    enum enumType {
        TYPE_IPV4        = AF_INET,
        TYPE_IPV6        = AF_INET6,
        TYPE_DOMAIN      = AF_UNIX,
        TYPE_UNSPECIFIED = 0xFF
    };

    union SocketInfo {
        struct sockaddr_in   IPV4Socket;
        struct sockaddr_in6  IPV6Socket;
        #ifndef __WIN32__
        struct sockaddr_un   DomainSocket;
        #endif
    };

    static bool IsIPV6Enabled() {
        return (m_isIPV6Enabled);
    }
    static void ClearIPV6Enabled() {
        m_isIPV6Enabled = false;
    }

    //------------------------------------------------------------------------
    // Constructors/Destructors
    //------------------------------------------------------------------------
public:
    NodeId();
    NodeId(const struct sockaddr_in& rInfo);
    NodeId(const struct sockaddr_in6& rInfo);
    #ifndef __WIN32__
    NodeId(const struct sockaddr_un& rInfo);
    #endif
    NodeId(const TCHAR strHostName[], const enumType defaultType = TYPE_UNSPECIFIED);
    NodeId(const TCHAR strHostName[], const uint16 nPortNumber, const enumType defaultType = TYPE_UNSPECIFIED);
    NodeId(const NodeId& rInfo);
	NodeId(const NodeId& rInfo, const uint16 portNumber);

    //------------------------------------------------------------------------
    // Public Methods
    //------------------------------------------------------------------------
public:
    NodeId::enumType Type() const {
        return (static_cast<NodeId::enumType>(m_structInfo.IPV4Socket.sin_family));
    }
    inline uint16 PortNumber() const {
        return (ntohs(m_structInfo.IPV4Socket.sin_port));
    }
    inline void PortNumber(const uint16 portNumber) {
        m_structInfo.IPV4Socket.sin_port = ntohs(portNumber);
    }
    inline bool IsValid() const {
        return (Type() != TYPE_UNSPECIFIED);
    }
    inline unsigned short Size() const {
        #ifndef __WIN32__
        return (m_structInfo.IPV4Socket.sin_family == AF_INET ? sizeof(struct sockaddr_in) : (m_structInfo.IPV6Socket.sin6_family == AF_INET6 ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_un)));
        #else
        return (m_structInfo.IPV4Socket.sin_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6));
        #endif
    }
    inline operator const struct sockaddr* () const {
        return (reinterpret_cast<const struct sockaddr*>(&(m_structInfo.IPV4Socket)));
    }
    inline operator const union SocketInfo& () const {
        return (m_structInfo);
    }
    inline bool operator!= (const NodeId& rInfo) const {
        return (!NodeId::operator== (rInfo));
    }

    string              HostName() const;
    void                HostName(const TCHAR strHostName[]);

    NodeId		AnyInterface() const;
    string              HostAddress() const;
    string              QualifiedName() const;
    NodeId              Origin() const;
    NodeId              Broadcast() const;
    bool                IsLocalInterface() const;
    bool                IsAnyInterface() const;
    bool                IsMulticast() const;

    bool                operator== (const NodeId& rInfo) const;

    NodeId&             operator= (const NodeId& rInfo);
    NodeId&             operator= (const struct sockaddr_in& rInfo);
    NodeId&             operator= (const struct sockaddr_in6& rInfo);
    NodeId&             operator= (const union SocketInfo& rInfo);

    #ifndef __WIN32__
    NodeId&             operator= (const struct sockaddr_un& rInfo);
    #endif


    //------------------------------------------------------------------------
    // Protected Methods
    //------------------------------------------------------------------------
protected:
    void                Resolve(const TextFragment& strName, const enumType defaultType);

private:
    mutable string	m_hostName;
    SocketInfo		m_structInfo;
    static bool         m_isIPV6Enabled;
};

}
} // namespace Core

#endif // __NODEID_H
