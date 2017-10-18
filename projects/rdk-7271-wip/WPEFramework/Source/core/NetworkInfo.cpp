#include "NetworkInfo.h"
#include "Trace.h"
#include "Serialization.h"
#include "Number.h"

#if defined(__WIN32__)
#include <winsock2.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>
#pragma comment(lib, "iphlpapi.lib")
#elif defined(__POSIX__)
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <list>
#include <ifaddrs.h>
#endif

#ifdef __APPLE__
#include <net/if_dl.h>
#endif

// Convert a string of binary bytes data (address) to a Hexadecimal string (output)
void ConvertMACToString(const uint8 address[], const uint8 length, const char delimiter, string& output)
{
    for (uint8 i = 0; i < length; i++) {
        // Reason for the low-level approch is performance.
        // In stead of using string operations, we know that each byte exists of 2 nibbles,
        // lets just translate these nibbles to Hexadecimal numbers and add them to the output.
        // This saves a setup of several string manipulation operations.
        uint8 highNibble = ((address[i] & 0xF0) >> 4);
        uint8 lowNibble = (address[i] & 0x0F);
        if ((i != 0) && (delimiter != '\0')) {
            output += delimiter;
        }
        output += static_cast<char>(highNibble + (highNibble >= 10 ? ('A' - 10) : '0'));
        output += static_cast<char>(lowNibble + (lowNibble >= 10 ? ('A' - 10) : '0'));
    }
}

namespace WPEFramework {

namespace Core {

#ifdef __WIN32__

/* Note: could also use malloc() and free() */
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

static uint16 AdapterCount = 0;
static PIP_ADAPTER_ADDRESSES LoadAdapterInfo(const uint16 adapterIndex)
{
    static PIP_ADAPTER_ADDRESSES _interfaceInfo = NULL;
    PIP_ADAPTER_ADDRESSES result = NULL;

    if (_interfaceInfo == NULL) {
        // Set the flags to pass to GetAdaptersAddresses
        ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

        // See how much we need to allcoate..
        ULONG outBufLen = 0;

        DWORD dwRetVal = ::GetAdaptersAddresses(AF_UNSPEC, flags, NULL, NULL, &outBufLen);

        // ALlocate the requested buffer
        _interfaceInfo = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(MALLOC(outBufLen));

        // Now get teh actual payload..
        dwRetVal = ::GetAdaptersAddresses(AF_UNSPEC, flags, NULL, _interfaceInfo, &outBufLen);

        if (dwRetVal == NO_ERROR) {
            PIP_ADAPTER_ADDRESSES pRunner = _interfaceInfo;

            while (pRunner != NULL) {
                AdapterCount++;
                pRunner = pRunner->Next;
            }

            if (AdapterCount == 0) {
                FREE(_interfaceInfo);
                _interfaceInfo = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(~0);
            }
        }
        else {
            // Oops failed..
            FREE(_interfaceInfo);

            _interfaceInfo = NULL;
        }
    }

    if (adapterIndex < AdapterCount) {
        uint16 index = adapterIndex;
        result = _interfaceInfo;

        while (index-- > 0) {
            ASSERT(result != NULL);

            result = result->Next;
        }

        ASSERT(result != NULL);
    }

    return (result);
}

    //  IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    //  IP_ADAPTER_PREFIX *pPrefix = NULL;

    //          printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",
    //                 pCurrAddresses->Length);
    //          printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
    //          printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

    //          pDnServer = pCurrAddresses->FirstDnsServerAddress;
    //          if (pDnServer) {
    //              for (i = 0; pDnServer != NULL; i++)
    //                  pDnServer = pDnServer->Next;
    //              printf("\tNumber of DNS Server Addresses: %d\n", i);
    //          } else
    //              printf("\tNo DNS Server Addresses\n");

    //          printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
    //          printf("\tDescription: %wS\n", pCurrAddresses->Description);
    //          printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

    //          printf("\tFlags: %ld\n", pCurrAddresses->Flags);
    //          printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
    //          printf("\tIfType: %ld\n", pCurrAddresses->IfType);
    //          printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
    //          printf("\tIpv6IfIndex (IPv6 interface): %u\n",
    //                 pCurrAddresses->Ipv6IfIndex);
    //          printf("\tZoneIndices (hex): ");
    //          for (i = 0; i < 16; i++)
    //              printf("%lx ", pCurrAddresses->ZoneIndices[i]);
    //          printf("\n");

    //          printf("\tTransmit link speed: %I64u\n", pCurrAddresses->TransmitLinkSpeed);
    //          printf("\tReceive link speed: %I64u\n", pCurrAddresses->ReceiveLinkSpeed);

    //          pPrefix = pCurrAddresses->FirstPrefix;
    //          if (pPrefix) {
    //              for (i = 0; pPrefix != NULL; i++)
    //                  pPrefix = pPrefix->Next;
    //              printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
    //          } else
    //              printf("\tNumber of IP Adapter Prefix entries: 0\n");

    IPV4AddressIterator::IPV4AddressIterator(const uint16 adapter)
        : _adapter(adapter)
        , _index(static_cast<uint16>(~0))
    {
        PIP_ADAPTER_ADDRESSES info = LoadAdapterInfo(_adapter);

        _section1 = 0;
        _section2 = 0;
        _section3 = 0;

        if (info != NULL) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = info->FirstUnicastAddress;
            while (pUnicast != NULL) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    _section1++;
                }

                pUnicast = pUnicast->Next;
            }

            PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = info->FirstMulticastAddress;
            while (pMulticast != NULL) {
                if (pMulticast->Address.lpSockaddr->sa_family == AF_INET) {
                    _section2++;
                }

                pMulticast = pMulticast->Next;
            }

            _section2 += _section1;

            PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = info->FirstAnycastAddress;
            while (pAnycast != NULL) {
                if (pAnycast->Address.lpSockaddr->sa_family == AF_INET) {
                    _section3++;
                }

                pAnycast = pAnycast->Next;
            }

            _section3 += _section2;
        }
    }

    NodeId IPV4AddressIterator::Address() const
    {
        NodeId result;

        ASSERT(IsValid());

        PIP_ADAPTER_ADDRESSES info = LoadAdapterInfo(_adapter);

        ASSERT(info != NULL);

        if (_index < _section1) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = info->FirstUnicastAddress;
            uint16 steps = _index + 1;

            while ((steps != 0) && (pUnicast != NULL)) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    steps--;
                }
                if (steps != 0) {
                    pUnicast = pUnicast->Next;
                }
            }

            ASSERT(pUnicast != NULL);
            ASSERT(pUnicast->Address.lpSockaddr->sa_family == AF_INET);

            result = *reinterpret_cast<sockaddr_in*>(pUnicast->Address.lpSockaddr);
        }
        else if ((_index >= _section1) && (_index < _section2)) {
            PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = info->FirstMulticastAddress;
            uint16 steps = _index - _section1 + 1;

            while ((steps != 0) && (pMulticast != NULL)) {
                if (pMulticast->Address.lpSockaddr->sa_family == AF_INET) {
                    steps--;
                }
                if (steps != 0) {
                    pMulticast = pMulticast->Next;
                }
            }

            ASSERT(pMulticast != NULL);
            ASSERT(pMulticast->Address.lpSockaddr->sa_family == AF_INET);

            result = *reinterpret_cast<sockaddr_in*>(pMulticast->Address.lpSockaddr);
        }
        else {
            PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = info->FirstAnycastAddress;
            uint16 steps = _index - _section2;

            while ((steps != 0) && (pAnycast != NULL)) {
                if (pAnycast->Address.lpSockaddr->sa_family == AF_INET) {
                    steps--;
                }
                if (steps != 0) {
                    pAnycast = pAnycast->Next;
                }
            }

            ASSERT(pAnycast != NULL);
            ASSERT(pAnycast->Address.lpSockaddr->sa_family == AF_INET);

            result = *reinterpret_cast<sockaddr_in*>(pAnycast->Address.lpSockaddr);
        }
        return (result);
    }

    uint16 IPV4AddressIterator::VLAN() const
    {
        // Information is not retrieved for windows yet..
        return (0);
    }

    IPV6AddressIterator::IPV6AddressIterator(const uint16 adapter)
        : _adapter(adapter)
        , _index(static_cast<uint16>(~0))
    {
        PIP_ADAPTER_ADDRESSES info = LoadAdapterInfo(_adapter);

        _section1 = 0;
        _section2 = 0;
        _section3 = 0;

        if (info != NULL) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = info->FirstUnicastAddress;
            while (pUnicast != NULL) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6) {
                    _section1++;
                }

                pUnicast = pUnicast->Next;
            }

            PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = info->FirstMulticastAddress;
            while (pMulticast != NULL) {
                if (pMulticast->Address.lpSockaddr->sa_family == AF_INET6) {
                    _section2++;
                }

                pMulticast = pMulticast->Next;
            }

            _section2 += _section1;

            PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = info->FirstAnycastAddress;
            while (pAnycast != NULL) {
                if (pAnycast->Address.lpSockaddr->sa_family == AF_INET6) {
                    _section3++;
                }

                pAnycast = pAnycast->Next;
            }

            _section3 += _section2;
        }
    }

    NodeId IPV6AddressIterator::Address() const
    {
        NodeId result;

        ASSERT(IsValid());

        PIP_ADAPTER_ADDRESSES info = LoadAdapterInfo(_adapter);

        ASSERT(info != NULL);

        if (_index < _section1) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = info->FirstUnicastAddress;
            uint16 steps = _index + 1;

            while ((steps != 0) && (pUnicast != NULL)) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6) {
                    steps--;
                }
                if (steps != 0) {
                    pUnicast = pUnicast->Next;
                }
            }

            ASSERT(pUnicast != NULL);
            ASSERT(pUnicast->Address.lpSockaddr->sa_family == AF_INET6);

            result = *reinterpret_cast<SOCKADDR_IN6*>(pUnicast->Address.lpSockaddr);
        }
        else if ((_index >= _section1) && (_index < _section2)) {
            PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = info->FirstMulticastAddress;
            uint16 steps = _index - _section1 + 1;

            while ((steps != 0) && (pMulticast != NULL)) {
                if (pMulticast->Address.lpSockaddr->sa_family == AF_INET6) {
                    steps--;
                }
                if (steps != 0) {
                    pMulticast = pMulticast->Next;
                }
            }

            ASSERT(pMulticast != NULL);
            ASSERT(pMulticast->Address.lpSockaddr->sa_family == AF_INET6);

            result = *reinterpret_cast<SOCKADDR_IN6*>(pMulticast->Address.lpSockaddr);
        }
        else {
            PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = info->FirstAnycastAddress;
            uint16 steps = _index - _section2;

            while ((steps != 0) && (pAnycast != NULL)) {
                if (pAnycast->Address.lpSockaddr->sa_family == AF_INET6) {
                    steps--;
                }
                if (steps != 0) {
                    pAnycast = pAnycast->Next;
                }
            }

            ASSERT(pAnycast != NULL);
            ASSERT(pAnycast->Address.lpSockaddr->sa_family == AF_INET6);

            result = *reinterpret_cast<SOCKADDR_IN6*>(pAnycast->Address.lpSockaddr);
        }
        return (result);
    }

    uint16 AdapterIterator::Count() const
    {
        // MAke sure it gets calculated if it was not yet calulated...
        // Lazy creation.
        LoadAdapterInfo(0);

        return (AdapterCount);
    }
    string AdapterIterator::Name() const
    {
        ASSERT(IsValid());
        string result(_T("Unknown"));
        PIP_ADAPTER_ADDRESSES info = LoadAdapterInfo(_index);

        if (info != NULL) {
            // info->AdapterName
            ToString(info->FriendlyName, result);
        }

        return (result);
    }
    string AdapterIterator::MACAddress(const char delimiter) const
    {
        ASSERT(IsValid());
        string result;
        PIP_ADAPTER_ADDRESSES info = LoadAdapterInfo(_index);

        if (info->PhysicalAddressLength != 0) {
            ConvertMACToString(info->PhysicalAddress, static_cast<uint8>(info->PhysicalAddressLength), delimiter, result);
        }
        return (result);
    }
	void AdapterIterator::MACAddress(uint8 buffer[], const uint8 length) const
	{
		ASSERT(IsValid());

		PIP_ADAPTER_ADDRESSES info = LoadAdapterInfo(_index);

		if (info->PhysicalAddressLength != 0) {
			ASSERT(length >= info->PhysicalAddressLength);
			::memcpy(buffer, info->PhysicalAddress, info->PhysicalAddressLength);
			if (length > info->PhysicalAddressLength) {
				::memset(&buffer[info->PhysicalAddressLength], 0, length - info->PhysicalAddressLength);
			}
		}
	}


#elif defined(__POSIX__)

struct __AdapterInfo__ {
    struct ipv4info : public sockaddr_in {
        ipv4info(const struct sockaddr_in& copy, const uint16 myFlags, const uint16 vlan)
            : sockaddr_in(copy)
            , flags(myFlags)
            , vlanNumber(vlan)
        {
        }
        uint16 flags;
        uint16 vlanNumber;
    };
    struct ipv6info : public sockaddr_in6 {
        ipv6info(const struct sockaddr_in6& copy, const uint16 myFlags, const uint16 vlan)
            : sockaddr_in6(copy)
            , flags(myFlags)
            , vlanNumber(vlan)
        {
        }
        uint16 flags;
        uint16 vlanNumber;
    };

    __AdapterInfo__(const string& ifName)
        : name(ifName)
    {
        int fd = socket(AF_INET, SOCK_DGRAM, 0);

        if (fd != -1) {
#ifdef __APPLE__
            ifaddrs* iflist;
            bool found = false;
            if (getifaddrs(&iflist) == 0) {
                for (ifaddrs* cur = iflist; cur; cur = cur->ifa_next) {
                    if ((cur->ifa_addr->sa_family == AF_LINK) &&
                            (strcmp(cur->ifa_name, ifName.c_str()) == 0) &&
                            cur->ifa_addr) {
                        sockaddr_dl* sdl = (sockaddr_dl*)cur->ifa_addr;
                        memcpy(MAC, LLADDR(sdl), sizeof(MAC));
                        found = true;
                        break;
                    }
                }

                freeifaddrs(iflist);
            }
#else
            struct ifreq ifr;
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, ifName.c_str(), IFNAMSIZ - 1);
            ioctl(fd, SIOCGIFHWADDR, &ifr);
            close(fd);
            memcpy(MAC, reinterpret_cast<uint8*>(ifr.ifr_addr.sa_data), sizeof(MAC));
#endif

        }

        // Check for multi homed IP
        size_t index = name.find(':', 0);
        if (index != string::npos) {
            name = name.substr(0, index);
        }
    }

    bool operator==(const __AdapterInfo__& RHS) const
    {
        return ((memcmp(MAC, RHS.MAC, sizeof(MAC)) == 0) && (name == RHS.name));
    }
    bool operator!=(const __AdapterInfo__& RHS) const
    {
        return !(operator==(RHS));
    }

    string name;
    uint8 MAC[6];

    std::list<ipv4info> ipv4Address;
    std::list<ipv6info> ipv6Address;
};

static std::list<__AdapterInfo__> adapters;

static const __AdapterInfo__* LoadInfo(const uint16 index)
{
    const __AdapterInfo__* result = NULL;

    if (adapters.size() == 0) {
        struct ifaddrs* ifa;

        if (getifaddrs(&ifa) != -1) {
            struct ifaddrs* current = ifa;

            while (current != NULL) {
                string name(current->ifa_name);
                uint16 vlanNumber = static_cast<uint16>(0);

                // Check for VLAN..
                size_t index = name.find('.', 0);
                if (index != string::npos) {
                    string textNumber(name.substr(index + 1, name.length() - (index + 1)));
                    NumberType<uint16, false> number(textNumber.c_str(), textNumber.length());
                    vlanNumber = number;

                    name = name.substr(0, index);
                }

                __AdapterInfo__ newEntry(name);
                std::list<__AdapterInfo__>::iterator listIndex = adapters.begin();

                // Find the right adapter, if it s already registered.
                while ((listIndex != adapters.end()) && (*listIndex != newEntry)) {
                    listIndex++;
                }

                if (listIndex == adapters.end()) {
                    adapters.push_front(newEntry);
                    listIndex = adapters.begin();
                }
                if (current->ifa_addr != NULL) {
                    if (current->ifa_addr->sa_family == AF_INET) {
                        __AdapterInfo__::ipv4info newRecord(*reinterpret_cast<sockaddr_in*>(current->ifa_addr), current->ifa_flags, vlanNumber);

                        listIndex->ipv4Address.push_back(newRecord);
                    }
                    else if (current->ifa_addr->sa_family == AF_INET6) {
                        __AdapterInfo__::ipv6info newRecord(*reinterpret_cast<sockaddr_in6*>(current->ifa_addr), current->ifa_flags, vlanNumber);

                        listIndex->ipv6Address.push_back(newRecord);
                    }
                }
                current = current->ifa_next;
            }

            freeifaddrs(ifa);
        }
    }

    uint16 count = index;
    std::list<__AdapterInfo__>::iterator listIndex = adapters.begin();

    // Find the right adapter, if it s already registered.
    while ((listIndex != adapters.end()) && (count != 0)) {
        listIndex++;
        count--;
    }

    if (listIndex != adapters.end()) {
        result = &(*listIndex);
    }

    return (result);
}

    IPV4AddressIterator::IPV4AddressIterator(const uint16 adapter)
        : _adapter(adapter)
        , _index(static_cast<uint16>(~0))
    {
        // Just a dummy load so we have the info
        const __AdapterInfo__* info = LoadInfo(_adapter);

        _section1 = 0;
        _section2 = 0;
        _section3 = 0;

        if (info != NULL) {
            // Best case, linux attaches 1 IPV4 to 1 adapter.
            _section1 = info->ipv4Address.size();
            _section2 = _section1;
            _section3 = _section2;
        }
    }
    NodeId IPV4AddressIterator::Address() const
    {
        NodeId result;
        // Just a dummy load so we have the info
        const __AdapterInfo__* info = LoadInfo(_adapter);

        uint16 count = _index;
        std::list<__AdapterInfo__::ipv4info>::const_iterator index(info->ipv4Address.begin());

        while ((count != 0) && (index != info->ipv4Address.end())) {
            index++;
            count--;
        }

        ASSERT(index != info->ipv4Address.end());

        result = (*index);

        return (result);
    }

    uint16 IPV4AddressIterator::VLAN() const
    {
        // Just a dummy load so we have the info
        const __AdapterInfo__* info = LoadInfo(_adapter);

        uint16 count = _index;
        std::list<__AdapterInfo__::ipv4info>::const_iterator index(info->ipv4Address.begin());

        while ((count != 0) && (index != info->ipv4Address.end())) {
            index++;
            count--;
        }

        ASSERT(index != info->ipv4Address.end());

        return (index->vlanNumber);
    }

    IPV6AddressIterator::IPV6AddressIterator(const uint16 adapter)
        : _adapter(adapter)
    {
        const __AdapterInfo__* info = LoadInfo(_adapter);

        _section1 = 0;
        _section2 = 0;
        _section3 = 0;

        if (info != NULL) {
            // Best case, linux attaches 1 IPV4 to 1 adapter.
            _section1 = info->ipv6Address.size();
            _section2 = _section1;
            _section3 = _section2;
        }
    }

    NodeId IPV6AddressIterator::Address() const
    {
        NodeId result;

        // Just a dummy load so we have the info
        const __AdapterInfo__* info = LoadInfo(_adapter);

        uint16 count = _index;
        std::list<__AdapterInfo__::ipv6info>::const_iterator index(info->ipv6Address.begin());

        while ((count != 0) && (index != info->ipv6Address.end())) {
            index++;
            count--;
        }

        ASSERT(index != info->ipv6Address.end());

        result = (*index);

        return result;
    }

    uint16 AdapterIterator::Count() const
    {
        // Just a dummy load so we have the info
        LoadInfo(0);
        return (adapters.size());
    }

    string AdapterIterator::Name() const
    {
        ASSERT(IsValid());

        const __AdapterInfo__* info = LoadInfo(_index);

        ASSERT(info != NULL);

        return (info->name);
    }

    string AdapterIterator::MACAddress(const char delimiter) const
    {
        string result;

        ASSERT(IsValid());

        const __AdapterInfo__* info = LoadInfo(_index);

        ASSERT(info != NULL);

        ConvertMACToString(info->MAC, sizeof(info->MAC), delimiter, result);

        return (result);
    }

    void AdapterIterator::MACAddress(uint8 buffer[], const uint8 length) const
    {
        ASSERT(IsValid());

        const __AdapterInfo__* info = LoadInfo(_index);

        ASSERT(info != NULL);

        ASSERT(length >= sizeof(info->MAC));

        ::memcpy(buffer, info->MAC, (length >= sizeof(info->MAC) ? sizeof(info->MAC) : length));

        if (length > sizeof(info->MAC)) {
            ::memset(&buffer[sizeof(info->MAC)], 0, length - sizeof(info->MAC));
        }
    }

#endif

    IPV4AddressIterator AdapterIterator::IPV4Unicast(const string& ifname)
    {
        IPV4AddressIterator result;

        if (ifname.empty() == false) {
            // Seems we neeed to bind to an interface
            // It might be an interface name, try to resolve it..
            while ((Next() == true) && (Name() != ifname)) {
                // Intentionally left empty...
            }

            if (IsValid() == true) {
                bool found = false;
                IPV4AddressIterator index(IPV4Addresses());

                while ((found == false) && (index.Next() == true)) {
                    NodeId current(index.Address());
                    if (current.IsMulticast() == false) {
                        result = index;
                        found = (current.IsLocalInterface() == false);
                    }
                }
            }
        }
        else {
            bool found = false;

            // time to find the public interface
            while ((found == false) && (Next() == true)) {
                IPV4AddressIterator index(IPV4Addresses());

                while ((found == false) && (index.Next() == true)) {
                    NodeId current(index.Address());
                    if ((current.IsMulticast() == false) || (current.IsLocalInterface() == false)) {
                        result = index;
                        found = true;
                    }
                }
            }
        }
        return (result);
    }
}
}
