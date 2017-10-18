#ifndef __IOPENCDMI_H
#define __IOPENCDMI_H

#include "Module.h"

namespace WPEFramework {
namespace Exchange {

    // This interface gives direct access to a OpenCDMi instance
    struct IOpenCDMi : virtual public Core::IUnknown {

        enum { ID = 0x00000055 };

        virtual ~IOpenCDMi() {}
        virtual uint32 Configure(PluginHost::IShell* framework) = 0;
    };
}
}

#endif // __OPENCDMI_
