#ifndef __ISYSTEMINFO_H
#define __ISYSTEMINFO_H

#include "Module.h"

namespace WPEFramework {
namespace PluginHost {

    // This interface gives direct access to a switchboard
    struct ISystemInfo : virtual public Core::IUnknown {

        enum { ID = 0x0000002C };

        enum network_type {
            UNKNOWN,
            IPV4,
            IPV6
        };

		static const TCHAR* ToString(const network_type value);

        enum system_state {
            POWERON       = 0,
            INITIALIZING  = 1,
            OPERATIONAL   = 100
        };

		static const TCHAR* ToString(const system_state value);

        struct INotification : virtual public Core::IUnknown {
            enum { ID = 0x0000002D };

            virtual ~INotification() {}

            // Some change happened with respect to the Network..
            virtual void Updated() = 0;
        };

		class EXTERNAL SystemState : public Core::EnumerateType<system_state> {
		public:
			inline SystemState() : Core::EnumerateType<system_state>() {
			}
			inline SystemState(const system_state value) : Core::EnumerateType<system_state>(value) {
			}
			inline SystemState(const SystemState& copy) : Core::EnumerateType<system_state>(copy) {
			}
			inline ~SystemState() {
			}

		public:
			string Data() const;
		};

        virtual ~ISystemInfo() {}

        virtual void Register(ISystemInfo::INotification* notification) = 0;
        virtual void Unregister(ISystemInfo::INotification* notification) = 0;

        // Hardware status 
        virtual system_state SystemState () const = 0;
        virtual system_state SystemState (const system_state newState) = 0;

        // Network information
        virtual string PublicIPAddress() const = 0;
        virtual network_type NetworkType() const = 0;
        virtual void PublicIPAddress(const string& ip) = 0;

        // Location information
        virtual string TimeZone() const = 0;
        virtual string Country() const = 0;
        virtual string Region() const = 0;
        virtual string City() const = 0;
        virtual void Location(const string& timeZone, const string& country, const string& region, const string& city) = 0;

        // Device specific identification.
        virtual uint8 Identifier(const uint8 length, uint8 buffer[]) const = 0;
        virtual void Identifier(const uint8 length, const uint8 buffer[]) = 0;

        // Time synchronisation reporting.
        virtual uint64 TimeSync() const = 0;
        virtual void TimeSync(const uint64 ticks) = 0;
    };
} // namespace PluginHost 
} // namespace WPEFramework

#endif // __ISYSTEMINFO_
