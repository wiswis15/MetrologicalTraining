#ifndef __IRESOURCECENTER_H
#define __IRESOURCECENTER_H

#include "Module.h"

namespace WPEFramework {
namespace Exchange {

    struct IResourceCenter : virtual public Core::IUnknown {
        enum { ID = 0x00000058 };

        enum hardware_state {
            FAILURE = 0,
            UNITIALIZED,
            INITIALIZING,
            OPERATIONAL,
            DEINITIALIZING,
        };

        struct INotification : virtual public Core::IUnknown {
            enum { ID = 0x00000059 };

            virtual ~INotification() {}

            // Signal changes on the subscribed namespace..
            virtual void StateChange(Exchange::IResourceCenter::hardware_state state) = 0;
        };

        virtual ~IResourceCenter() {}

        virtual uint32 Configure(const string& Config) = 0;
        virtual void Register(IResourceCenter::INotification* provisioning) = 0;
        virtual void Unregister(IResourceCenter::INotification* provisioning) = 0;

        virtual uint8 Identifier(const uint8 maxLength, uint8 buffer[]) const = 0;
        virtual hardware_state HardwareState() const = 0;
    };
}
}

#endif // __IRESOURCECENTER_H
