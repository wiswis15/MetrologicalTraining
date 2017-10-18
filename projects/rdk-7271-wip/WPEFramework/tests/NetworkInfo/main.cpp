#include "../../generics/generics.h"
#include <iostream>

int main()
{
    WPEFramework::Core::AdapterIterator adapters;

    while (adapters.Next() == true) {
        string current = adapters.Name();
        std::cout << _T("Name:              ") << current << std::endl;
        std::cout << _T("MAC:               ") << adapters.MACAddress(':') << std::endl;

        // Print all IPV4 addresses associated with this adapter.
        WPEFramework::Core::IPV4AddressIterator addresses(adapters.IPV4Addresses());

        while (addresses.Next() == true) {
            WPEFramework::Core::NodeId address = addresses.Address();

            if (addresses.IsUnicast() == true) {
                std::cout << _T("IPV4 UNICAST[") << std::dec << addresses.VLAN() << "]:   " << address.HostAddress() << std::endl;
            }
            else if (addresses.IsMulticast() == true) {
                std::cout << _T("IPV4 MULTICAST[") << std::dec << addresses.VLAN() << "]:   " << address.HostAddress() << std::endl;
            }
            else if (addresses.IsAnycast() == true) {
                std::cout << _T("IPV4 ANYCAST[") << std::dec << addresses.VLAN() << "]:   " << address.HostAddress() << std::endl;
            }
            else {
                ASSERT(false);
            }
        }

        WPEFramework::Core::IPV6AddressIterator addressesV6(adapters.IPV6Addresses());

        while (addressesV6.Next() == true) {
            WPEFramework::Core::NodeId address = addressesV6.Address();

            if (addressesV6.IsUnicast() == true) {
                std::cout << _T("IPV6 UNICAST:      ") << address.HostAddress() << std::endl;
            }
            else if (addressesV6.IsMulticast() == true) {
                std::cout << _T("IPV6 MULTICAST:    ") << address.HostAddress() << std::endl;
            }
            else if (addressesV6.IsAnycast() == true) {
                std::cout << _T("IPV6 ANYCAST:      ") << address.HostAddress() << std::endl;
            }
            else {
                ASSERT(false);
            }
        }

        std::cout << std::endl;
    }

    WPEFramework::Core::Singleton::Dispose();
    return 0;
}
