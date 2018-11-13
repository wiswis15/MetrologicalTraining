#pragma once

#include "Module.h"
#include <interfaces/ICapture.h>

namespace WPEFramework {
namespace Plugin {

    class Plugin01 : public PluginHost::IPlugin, public PluginHost::IWeb {
    private:
        Plugin01(const Plugin01&) = delete;
        Plugin01& operator=(const Plugin01&) = delete;

    public:
        Plugin01()
            : _skipURL(0)
        {
        }

        virtual ~Plugin01()
        {
        }

        BEGIN_INTERFACE_MAP(Plugin01)
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
        uint8_t _skipURL;
    };

} // Namespace Plugin.
}

