#ifndef __IBROWSER_H
#define __IBROWSER_H

#include "Module.h"

namespace WPEFramework {
namespace Exchange {

    // This interface gives direct access to a Browser to change
    // Browser specific properties like displayed URL.
    struct IBrowser : virtual public Core::IUnknown {
        enum { ID = 0x00000040 };

        struct INotification : virtual public Core::IUnknown {
            enum { ID = 0x00000041 };

            virtual ~INotification() {}

            // Signal changes on the subscribed namespace..
            virtual void LoadFinished(const string& URL) = 0;
            virtual void URLChanged(const string& URL) = 0;
            virtual void Hidden(const bool hidden) = 0;
        };

        virtual ~IBrowser() {}

        // Change the currenly displayed URL by the browser.
        virtual void SetURL(const string& URL) = 0;
        virtual string GetURL() const = 0;
        virtual uint32 GetFPS() const = 0;

        virtual void Register(IBrowser::INotification* sink) = 0;
        virtual void Unregister(IBrowser::INotification* sink) = 0;

        // There might be dependencies on which the browser should 
        // wait, like hardware :-) before it can continue to start
        // through. If all conditions have been met, the browser
        // will be GO !!!
        virtual void Go() = 0;

        virtual void Hide(const bool hidden) = 0;
    };
}
}

#endif // __IBROWSER_H
