#pragma once

#include "Module.h"
#include <interfaces/ICapture.h>
#include <compositor/Client.h>

namespace WPEFramework {
namespace Plugin {

    class Plugin02 : public PluginHost::IPlugin, public PluginHost::IWeb {
    private:
        Plugin02(const Plugin02&) = delete;
        Plugin02& operator=(const Plugin02&) = delete;

    public:
        Plugin02();
        virtual ~Plugin02();

        BEGIN_INTERFACE_MAP(Plugin02)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IWeb)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        virtual const string Initialize(PluginHost::IShell* service);
        virtual void Deinitialize(PluginHost::IShell* service);
        virtual string Information() const;

        //	IWeb methods
        // -------------------------------------------------------------------------------------------------------
        virtual void Inbound(Web::Request& request);
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

    private:
        uint8_t m_skipURL;
        Compositor::IDisplay* m_display;
    };

} // Namespace Plugin.
}

