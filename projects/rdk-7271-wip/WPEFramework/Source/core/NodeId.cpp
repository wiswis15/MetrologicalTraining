// SocketPortSettings.cpp: implementation of the NodeId class.
//
//////////////////////////////////////////////////////////////////////

#include "Portability.h"
#include "string.h"
#include "Trace.h"
#include "TextFragment.h"
#include "Serialization.h"

#ifdef __UNIX__
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#define ERRORRESULT        errno
#endif

#ifdef __WIN32__
#include <Ws2tcpip.h>
#define ERRORRESULT        ::WSAGetLastError ()
#endif

#include "NodeId.h"

namespace WPEFramework {
namespace Core {

/* static */ bool NodeId::m_isIPV6Enabled = true;

//----------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//----------------------------------------------------------------------------
NodeId::NodeId() :
    m_hostName() {
    // Fill it with FF's
    memset(&m_structInfo, 0xFF, sizeof(m_structInfo));
}

NodeId::NodeId(const struct sockaddr_in& rInfo) :
    m_hostName() {

    *this = rInfo;

    ASSERT(m_structInfo.IPV4Socket.sin_family == AF_INET);
}

NodeId::NodeId(const struct sockaddr_in6& rInfo) :
    m_hostName() {

    *this = rInfo;

    ASSERT(m_structInfo.IPV6Socket.sin6_family == AF_INET6);
}


#ifndef __WIN32__
NodeId::NodeId(const struct sockaddr_un& rInfo) :
    m_hostName(rInfo.sun_path) {

    *this = rInfo;

    ASSERT(m_structInfo.DomainSocket.sun_family == AF_UNIX);
}
#endif

NodeId::NodeId(
    const TCHAR   strHostName[],
    const uint16  nPortNumber,
    const enumType defaultType) :
    m_hostName() {

    m_structInfo.IPV4Socket.sin_family = TYPE_UNSPECIFIED;

    // Convert the name to an IP structure address
    Resolve(Core::TextFragment(strHostName), defaultType);

    ASSERT(nPortNumber < 0xFFFF);

    // Set the Port number used. (Struct of IPV4 and IPV6 are equeal w.r.t. port number)
    m_structInfo.IPV4Socket.sin_port = htons(nPortNumber);

    ASSERT ((m_structInfo.IPV4Socket.sin_family == TYPE_IPV4) || (m_structInfo.IPV4Socket.sin_family == TYPE_IPV6) || (m_structInfo.IPV4Socket.sin_family == TYPE_UNSPECIFIED));
}

NodeId::NodeId(
    const TCHAR strHostName[],
    const enumType defaultType) :
    m_hostName () {

    m_structInfo.IPV4Socket.sin_family = TYPE_UNSPECIFIED;

    #ifndef __WIN32__
    if (strchr(strHostName, '/') != NULL) {
        // Seems like we have a path, so a domain socket is required....
        m_hostName = strHostName;

        m_structInfo.DomainSocket.sun_family = AF_UNIX;
        strncpy (m_structInfo.DomainSocket.sun_path, strHostName, sizeof (m_structInfo.DomainSocket.sun_path));
        m_structInfo.DomainSocket.sun_path[sizeof(m_structInfo.DomainSocket.sun_path)-1] = '\0';
    } else
    #endif
    {
        const TCHAR* portNumber;
        const TCHAR* end;
        const TCHAR* start(strchr(strHostName, '['));

        // If a port number is mentioned on IPV6, the address is surrounded by square blocks, check for these block characters.
        if ((start != nullptr) && ((end = strrchr(strHostName, ']')) != nullptr)) {
            // This is a IPV6 address..
            Resolve(TextFragment(strHostName,

                                 static_cast<uint32>(start - strHostName) + 1,
                                 static_cast<uint32>((end - start) - 1) / static_cast<uint32>(sizeof(TCHAR))),
                    TYPE_IPV6);

            // See if there is a colon, the colon might follow a port numnber at the end..
            if ((portNumber = strrchr(&(end[1]), ':')) != NULL) {
                // Seems we have a number..
                m_structInfo.IPV6Socket.sin6_port = htons(Core::NumberType<uint16>(&(portNumber[1]), static_cast<uint32>(strlen(&(portNumber[1])))).Value());
            }
        }
        // See if there is a colon, the colon might follow a port numnber at the end and make sure there is only 1 colon!!!
        else if ( ((portNumber = strrchr(strHostName, ':')) != NULL) && (strchr(strHostName, ':') == portNumber) ) {
            // Convert the name to an IP structure address
            Resolve(TextFragment(strHostName, 0, static_cast<uint32>(portNumber - strHostName) / static_cast<uint32>(sizeof(TCHAR))), defaultType);

			// Seems we have a number..
			if (m_structInfo.IPV6Socket.sin6_family == AF_INET6)
			{
				m_structInfo.IPV6Socket.sin6_port = htons(Core::NumberType<uint16>(&(portNumber[1]), static_cast<uint32>(strlen(&(portNumber[1])))).Value());
			}
			else if (m_structInfo.IPV4Socket.sin_family == AF_INET)
			{
				m_structInfo.IPV4Socket.sin_port = htons(Core::NumberType<uint16>(&(portNumber[1]), static_cast<uint32>(strlen(&(portNumber[1])))).Value());
			}

        } else {
            // Convert the name to an IP structure address
            Resolve(TextFragment(strHostName), defaultType);

            // Set the Port number used.
            m_structInfo.IPV4Socket.sin_port = 0;
        }

        ASSERT ( (m_structInfo.IPV4Socket.sin_family == AF_INET) || (m_structInfo.IPV4Socket.sin_family == AF_INET6) || (m_structInfo.IPV4Socket.sin_family == TYPE_UNSPECIFIED) );
    }
}

NodeId::NodeId(const NodeId& rInfo) {

    *this = rInfo;
}

NodeId::NodeId (const NodeId& rInfo, const uint16 portNumber) {
	*this = rInfo;
	PortNumber(portNumber);
}

//----------------------------------------------------------------------------
// PUBLIC METHODS
//----------------------------------------------------------------------------

bool
NodeId::operator== (const NodeId& rInfo) const {
    if (m_structInfo.IPV4Socket.sin_family == rInfo.m_structInfo.IPV4Socket.sin_family) {
        if (m_structInfo.IPV4Socket.sin_family == AF_INET) {
            return ((m_structInfo.IPV4Socket.sin_addr.s_addr == rInfo.m_structInfo.IPV4Socket.sin_addr.s_addr) &&
                    (m_structInfo.IPV4Socket.sin_port        == rInfo.m_structInfo.IPV4Socket.sin_port));
        } else if (m_structInfo.IPV6Socket.sin6_family == AF_INET6) {

            #ifdef __WIN32__
            return ((memcmp(m_structInfo.IPV6Socket.sin6_addr.u.Byte, rInfo.m_structInfo.IPV6Socket.sin6_addr.u.Byte, sizeof (m_structInfo.IPV6Socket.sin6_addr.u.Byte)) == 0) &&
                    (m_structInfo.IPV6Socket.sin6_port == rInfo.m_structInfo.IPV6Socket.sin6_port));
            #else
            return ((memcmp(m_structInfo.IPV6Socket.sin6_addr.s6_addr, rInfo.m_structInfo.IPV6Socket.sin6_addr.s6_addr, sizeof (m_structInfo.IPV6Socket.sin6_addr.s6_addr)) == 0) &&
                    (m_structInfo.IPV6Socket.sin6_port == rInfo.m_structInfo.IPV6Socket.sin6_port));
            #endif
        }
        #ifndef __WIN32__
        else {
            return (strcmp (m_structInfo.DomainSocket.sun_path, rInfo.m_structInfo.DomainSocket.sun_path) == 0);
        }
        #endif
    }

    return (false);
}

NodeId&
NodeId::operator= (const NodeId& rInfo) {

    // Copy the struct info
    memcpy(&m_structInfo, &rInfo.m_structInfo, sizeof(m_structInfo));

    m_hostName = rInfo.m_hostName;

    // Give back our-selves.
    return (*this);
}

NodeId&
NodeId::operator= (const struct sockaddr_in& rInfo) {

    // Copy the struct info
    memcpy(&m_structInfo.IPV4Socket, &rInfo, sizeof(m_structInfo.IPV4Socket));

    m_hostName.clear();

    // Give back our-selves.
    return (*this);
}

NodeId&
NodeId::operator= (const struct sockaddr_in6& rInfo) {

    // Copy the struct info
    memcpy(&m_structInfo.IPV6Socket, &rInfo, sizeof(m_structInfo.IPV6Socket));

    m_hostName.clear();

    // Give back our-selves.
    return (*this);
}

#ifndef __WIN32__
NodeId&
NodeId::operator= (const struct sockaddr_un& rInfo) {
    // Copy the struct info
    memcpy(&m_structInfo.DomainSocket, &rInfo, sizeof(m_structInfo.DomainSocket));

    m_hostName = rInfo.sun_path;

    // Give back our-selves.
    return (*this);
}
#endif

NodeId&
NodeId::operator= (const union SocketInfo& rInfo) {

    // Copy the struct info
    memcpy(&m_structInfo, &rInfo, sizeof(m_structInfo));

    m_hostName.clear();

    // Give back our-selves.
    return (*this);
}

bool NodeId::IsMulticast () const {
    if (m_structInfo.IPV4Socket.sin_family == AF_INET) {
        #ifdef __WIN32__
        return (((m_structInfo.IPV4Socket.sin_addr.S_un.S_un_b.s_b1 & 0xF0) & 0xE0) == 0xE0);
        #else
        return (((m_structInfo.IPV4Socket.sin_addr.s_addr) & 0xE0000000) == 0xE0000000);
        #endif
    } else if (m_structInfo.IPV4Socket.sin_family == AF_INET6) {
        return (false);
    }

    return (false);
}

string
NodeId::QualifiedName() const {
    if (m_structInfo.IPV4Socket.sin_family == AF_INET) {
        Core::NumberType<uint16,false, BASE_DECIMAL> convertedNumber (ntohs(m_structInfo.IPV4Socket.sin_port));

        return (HostName() + _T(":") + convertedNumber.Text());
    } else if (m_structInfo.IPV6Socket.sin6_family == AF_INET6) {
        Core::NumberType<uint16, false, BASE_DECIMAL> convertedNumber(ntohs(m_structInfo.IPV6Socket.sin6_port));

        return (_T("[") + HostName() + _T("]:") + convertedNumber.Text());
    }

    return (m_hostName);
}

string
NodeId::HostName() const {
    if ((IsValid() == true) && (m_hostName.empty() == true)) {
        char host[NI_MAXHOST], service[NI_MAXSERV];

        int error = getnameinfo(
                        reinterpret_cast<const struct sockaddr*>(&(m_structInfo.IPV4Socket)), Size(),
                        host, sizeof(host),
                        service, sizeof(service), NI_NUMERICSERV);

        if (error == 0) {
            m_hostName = ToString(host);
        }
        else {
            TRACE_L1 ("Function getnameinfo failed, error: %s", gai_strerror(error));
        }

        if(m_hostName.empty() == true) {
            m_hostName = HostAddress();
        }
    }

    return (m_hostName);
}

string
NodeId::HostAddress() const {
    string result;

    if (m_structInfo.IPV4Socket.sin_family == AF_UNIX) {
        result = m_hostName;
    } else if (IsValid() == true) {
        if (m_structInfo.IPV4Socket.sin_family == AF_INET) {
            uint32  nAddress = m_structInfo.IPV4Socket.sin_addr.s_addr;

            TCHAR identifier[16];

            #ifdef __WIN32__
#pragma warning (disable: 4996)
            #endif

            // Lets resolve it four digits.
            _stprintf(identifier, _T("%d.%d.%d.%d"), ((nAddress & 0x000000FF) >> 0),
                      ((nAddress & 0x0000FF00) >> 8),
                      ((nAddress & 0x00FF0000) >> 16),
                      ((nAddress & 0xFF000000) >> 24));

            #ifdef __WIN32__
#pragma warning (default: 4996)
            #endif

            result = string(identifier);
        } else if (m_structInfo.IPV6Socket.sin6_family == AF_INET6) {
            char buffer[INET6_ADDRSTRLEN];
            if (getnameinfo(*this, Size(), buffer, sizeof(buffer), 0, 0, NI_NUMERICHOST) == 0) {
                result = string(buffer);
            }
        }
    }
    return (result);
}

NodeId
NodeId::AnyInterface() const {
    Core::NodeId result;

    switch (Type()) {
    case TYPE_IPV4:
        result.m_structInfo.IPV4Socket.sin_family = AF_INET;
        result.m_structInfo.IPV4Socket.sin_addr.s_addr = 0;
        result.m_structInfo.IPV4Socket.sin_port = 0;
        break;
    case TYPE_IPV6:
        result.m_structInfo.IPV6Socket.sin6_family = AF_INET6;
        result.m_structInfo.IPV6Socket.sin6_port = 0;
        memset (result.m_structInfo.IPV6Socket.sin6_addr.s6_addr, 0, sizeof(result.m_structInfo.IPV6Socket.sin6_addr.s6_addr));
        break;
        #ifndef __WIN32__
    case TYPE_DOMAIN:
        result.m_structInfo.DomainSocket.sun_family = AF_UNIX;
        result.m_structInfo.DomainSocket.sun_path[0] = '\0';
        break;
        #endif

    default:
        break;
    }

    return (result);
}
bool
NodeId::IsAnyInterface() const {
    if (m_structInfo.IPV4Socket.sin_family  == AF_INET) {
        return (m_structInfo.IPV4Socket.sin_addr.s_addr == 0);
    } else if (m_structInfo.IPV6Socket.sin6_family == AF_INET6) {
        uint8 index = sizeof (m_structInfo.IPV6Socket.sin6_addr.s6_addr);
        while (index > 0) {
            if (m_structInfo.IPV6Socket.sin6_addr.s6_addr[index-1] != 0) {
                break;
            }
            index--;
        }
        return (index == 0);
    }
    #ifndef __WIN32__
    else if (m_structInfo.DomainSocket.sun_family == AF_UNIX) {
        return (m_structInfo.DomainSocket.sun_path[0] == '\0');
    }
    #endif

    return (false);
}

bool
NodeId::IsLocalInterface() const {
    if (m_structInfo.IPV4Socket.sin_family  == AF_INET) {
        return ((m_structInfo.IPV4Socket.sin_addr.s_addr & 0xFF000000) == 0x7F000000);
    } else if (m_structInfo.IPV6Socket.sin6_family == AF_INET6) {
        uint8 index = sizeof (m_structInfo.IPV6Socket.sin6_addr.s6_addr) - 1;

        if (m_structInfo.IPV6Socket.sin6_addr.s6_addr[index] == '1') {

            while (index > 0) {
                if (m_structInfo.IPV6Socket.sin6_addr.s6_addr[index-1] != 0) {
                    break;
                }
                index--;
            }
        }
        return (index == 0);
    }
    #ifndef __WIN32__
    else if (m_structInfo.DomainSocket.sun_family == AF_UNIX) {
        return (true);
    }
    #endif
    return (false);
}

NodeId
NodeId::Origin() const {

    NodeId response;

    if (m_structInfo.IPV4Socket.sin_family == AF_INET) {
        response.m_structInfo.IPV4Socket.sin_family = AF_INET;
        response.m_structInfo.IPV4Socket.sin_addr.s_addr = 0;
        response.m_structInfo.IPV4Socket.sin_port = 0;
        response.m_hostName.clear();
    } else if (m_structInfo.IPV6Socket.sin6_family == AF_INET6) {
        response.m_structInfo.IPV6Socket.sin6_family = AF_INET;
        response.m_structInfo.IPV4Socket.sin_addr.s_addr = 0;
        response.m_structInfo.IPV6Socket.sin6_port = 0;

        response.m_hostName.clear();
    }
    #ifndef __WIN32__
    else    {
        struct sockaddr_un      domainSocket;
        memset(&domainSocket, 0x00, sizeof(domainSocket));
        domainSocket.sun_family = AF_UNIX;
        response = domainSocket;
    }
    #endif

    return (response);
}

NodeId
NodeId::Broadcast() const {
    struct sockaddr_in  l_Result;
    unsigned long     l_Address = m_structInfo.IPV4Socket.sin_addr.s_addr;

    ASSERT (m_structInfo.IPV4Socket.sin_family == AF_INET);

    memcpy(&l_Result, &m_structInfo, sizeof(l_Result));

    // The broadcast address is formed by setting the host portion
    // of a network address to all 1's
    if((l_Address & 0x80000000L) == 0) {
        // CLASS A network.
        l_Address |= 0xFFFFFF00L;
    } else if((l_Address & 0x40000000L) == 0) {
        // CLASS B network.
        l_Address |= 0xFFFF0000L;
    } else {
        // CLASS C network.
        l_Address |= 0xFF000000L;
    }

    l_Result.sin_addr.s_addr = l_Address;

    return (NodeId(l_Result));
}

//----------------------------------------------------------------------------
// PROTECTED METHODS
//----------------------------------------------------------------------------

void
NodeId::Resolve(const TextFragment& strHostName, const enumType defaultType) {

    if (IsValid() == false) {

        struct addrinfo  hints;
        struct addrinfo* result;
        std::string  text;
        ToString(strHostName.Text().c_str(), text);

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = (m_isIPV6Enabled == false ? AF_INET : (defaultType == NodeId::TYPE_UNSPECIFIED ? AF_UNSPEC : defaultType));    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_STREAM;  /* Datagram socket */
        hints.ai_flags = AI_PASSIVE;      /* For wildcard IP address */
        hints.ai_protocol = IPPROTO_TCP;  /* Only TCP protocol */

        int error = getaddrinfo(text.c_str(), nullptr, &hints, &result);

        if ((error == 0) && (result != nullptr)) {

            /* getaddrinfo() returns a list of address structures. */
            /* Jut pick the first one.. */
            memcpy(&(m_structInfo.IPV4Socket), result->ai_addr, (result->ai_family == AF_INET6 ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in)));
            m_structInfo.IPV4Socket.sin_family = result->ai_family;

            freeaddrinfo(result);           /* No longer needed */
        } else {
            TRACE_L1("Function ::getaddrinfo() for %s failed, error %s", text.c_str(), gai_strerror(error));
        }
    }
}

}
}
