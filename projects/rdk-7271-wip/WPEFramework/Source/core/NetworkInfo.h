#ifndef __NETWORKINFO_H
#define __NETWORKINFO_H

#include "Module.h"
#include "Portability.h"
#include "NodeId.h"

namespace WPEFramework {
namespace Core {
    class EXTERNAL IPV4AddressIterator {
    public:
        inline IPV4AddressIterator()
            : _section1(static_cast<uint16>(~0))
            , _section2(static_cast<uint16>(~0))
            , _section3(static_cast<uint16>(~0))
            , _adapter(static_cast<uint16>(~0))
            , _index(static_cast<uint16>(~0))
        {
        }
        IPV4AddressIterator(const uint16 adapter);
        inline IPV4AddressIterator(const IPV4AddressIterator& copy)
            : _section1(copy._section1)
            , _section2(copy._section2)
            , _section3(copy._section3)
            , _adapter(copy._adapter)
            , _index(copy._index)
        {
        }
        inline ~IPV4AddressIterator()
        {
        }

        inline IPV4AddressIterator& operator=(const IPV4AddressIterator& RHS)
        {
            _adapter = RHS._adapter;
            _index = RHS._index;
            _section1 = RHS._section1;
            _section2 = RHS._section2;
            _section3 = RHS._section3;
            return (*this);
        }

    public:
        inline bool IsValid() const
        {
            return (_index < Count());
        }
        inline void Reset()
        {
            _index = static_cast<uint16>(~0);
        }
        inline bool Next()
        {
            if (_index == static_cast<uint16>(~0)) {
                _index = 0;
            }
            else if (_index < Count()) {
                _index++;
            }

            return (IsValid());
        }
        inline bool IsUnicast() const
        {
            return (_index < _section1);
        }
        inline bool IsMulticast() const
        {
            return ((_index >= _section1) && (_index < _section2));
        }
        inline bool IsAnycast() const
        {
            return ((_index >= _section2) && (_index < _section3));
        }
        inline uint16 Count() const
        {
            return (_section3);
        }

        NodeId Address() const;
        uint16 VLAN() const;

    private:
        uint16 _section1;
        uint16 _section2;
        uint16 _section3;
        uint16 _adapter;
        uint16 _index;
    };

    class EXTERNAL IPV6AddressIterator {
    public:
        inline IPV6AddressIterator()
            : _section1(static_cast<uint16>(~0))
            , _section2(static_cast<uint16>(~0))
            , _section3(static_cast<uint16>(~0))
            , _adapter(static_cast<uint16>(~0))
            , _index(static_cast<uint16>(~0))
        {
        }
        IPV6AddressIterator(const uint16 adapter);
        inline IPV6AddressIterator(const IPV6AddressIterator& copy)
            : _section1(copy._section1)
            , _section2(copy._section2)
            , _section3(copy._section3)
            , _adapter(copy._adapter)
            , _index(copy._index)
        {
        }
        inline ~IPV6AddressIterator()
        {
        }

        inline IPV6AddressIterator& operator=(const IPV6AddressIterator& RHS)
        {
            _adapter = RHS._adapter;
            _index = RHS._index;
            _section1 = RHS._section1;
            _section2 = RHS._section2;
            _section3 = RHS._section3;
            return (*this);
        }

    public:
        inline bool IsValid() const
        {
            return (_index < Count());
        }
        inline void Reset()
        {
            _index = static_cast<uint16>(~0);
        }
        inline bool Next()
        {
            if (_index == static_cast<uint16>(~0)) {
                _index = 0;
            }
            else if (_index < Count()) {
                _index++;
            }

            return (IsValid());
        }
        inline bool IsUnicast() const
        {
            return (_index < _section1);
        }
        inline bool IsMulticast() const
        {
            return ((_index >= _section1) && (_index < _section2));
        }
        inline bool IsAnycast() const
        {
            return ((_index >= _section2) && (_index < _section3));
        }
        inline uint16 Count() const
        {
            return (_section3);
        }

        NodeId Address() const;

    private:
        uint16 _section1;
        uint16 _section2;
        uint16 _section3;
        uint16 _adapter;
        uint16 _index;
    };

    class EXTERNAL AdapterIterator {
    public:
        inline AdapterIterator()
            : _index(static_cast<uint16>(~0))
        {
        }
        inline AdapterIterator(const string& name)
            : _index(static_cast<uint16>(~0))
        {
            while ((Next() == true) && (Name() != name)) /* intentionally left blank */
                ;

            if (IsValid() == false) {
                Reset();
            }
        }
        inline AdapterIterator(const AdapterIterator& copy)
            : _index(copy._index)
        {
        }
        inline ~AdapterIterator()
        {
        }

        inline AdapterIterator& operator=(const AdapterIterator& RHS)
        {
            _index = RHS._index;
            return (*this);
        }

    public:
        inline uint16 Index() const
        {
            return (_index);
        }
        inline bool IsValid() const
        {
            return (_index < Count());
        }
        inline void Reset()
        {
            _index = static_cast<uint16>(~0);
        }
        inline bool Next()
        {
            if (_index == static_cast<uint16>(~0)) {
                _index = 0;
            }
            else if (_index < Count()) {
                _index++;
            }

            return (IsValid());
        }
        inline IPV4AddressIterator IPV4Addresses() const
        {
            return (IPV4AddressIterator(_index));
        }
        inline IPV6AddressIterator IPV6Addresses() const
        {
            return (IPV6AddressIterator(_index));
        }
        IPV4AddressIterator IPV4Unicast(const string& ifname);

        uint16 Count() const;
        string Name() const;
        string MACAddress(const char delimiter) const;
        void MACAddress(uint8 buffer[], const uint8 length) const;

    private:
        uint16 _index;
    };
}
}

#endif // __NETWORKINFO_H
