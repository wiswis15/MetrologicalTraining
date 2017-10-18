#ifndef __PLUGIN_COMPOSITOR_H
#define __PLUGIN_COMPOSITOR_H

#include "Module.h"
#include <interfaces/IResourceCenter.h>
#include <interfaces/IComposition.h>

namespace WPEFramework {
namespace Plugin {
    class Compositor : public PluginHost::IPlugin, public PluginHost::IWeb {
    private:
        Compositor(const Compositor&) = delete;
        Compositor& operator=(const Compositor&) = delete;

    private:
        class Notification : public PluginHost::IPlugin::INotification,
                             public Exchange::IResourceCenter::INotification,
                             public Exchange::IComposition::INotification {
        private:
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
            Notification(Compositor* parent)
                : _parent(*parent)
            {
            }
            virtual ~Notification(){};

            //   IPlugin INotification methods
            // -------------------------------------------------------------------------------------------------------
            virtual void StateChange(PluginHost::IShell* plugin) override
            {
                _parent.StateChange(plugin);
            }

            //   IResourceCenter INotification methods
            // -------------------------------------------------------------------------------------------------------
            virtual void StateChange(Exchange::IResourceCenter::hardware_state state) override
            {
                _parent.StateChange(state);
            }

            //   IComposition INotification methods
            // -------------------------------------------------------------------------------------------------------
            virtual void Attached(Exchange::IComposition::IClient* client) override
            {
                _parent.Attached(client);
            }
            virtual void Detached(Exchange::IComposition::IClient* client) override
            {
                _parent.Detached(client);
            }

            BEGIN_INTERFACE_MAP(PluginSink)
            INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
            INTERFACE_ENTRY(Exchange::IResourceCenter::INotification)
            INTERFACE_ENTRY(Exchange::IComposition::INotification)
            END_INTERFACE_MAP

        private:
            Compositor& _parent;
        };

    public:
        class Config : public Core::JSON::Container {
        public:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            Config()
                : Core::JSON::Container()
                , OutOfProcess(true)
                , System(_T("Controller"))
                , Locator("libplatformplugin.so")
            {
                Add(_T("outofprocess"), &OutOfProcess);
                Add(_T("system"), &System);
                Add(_T("locator"), &Locator);
                Add(_T("workdir"), &WorkDir);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::Boolean OutOfProcess;
            Core::JSON::String System;
            Core::JSON::String Locator;
            Core::JSON::String WorkDir;
        };

    public:
        class Data : public Core::JSON::Container {
        private:
            Data(const Data&) = delete;
            Data& operator=(const Data&) = delete;

        public:
            Data()
                : Core::JSON::Container()
                , Clients()
            {
                Add(_T("clients"), &Clients);
            }

            virtual ~Data()
            {
            }

        public:
            Core::JSON::ArrayType<Core::JSON::String> Clients;
        };

    public:
        Compositor();
        virtual ~Compositor();

    public:
        BEGIN_INTERFACE_MAP(Compositor)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        INTERFACE_AGGREGATE(Exchange::IResourceCenter, _resourceCenter)
        INTERFACE_AGGREGATE(Exchange::IComposition, _composition)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        virtual const string Initialize(PluginHost::IShell* service) override;
        virtual void Deinitialize(PluginHost::IShell* service) override;
        virtual string Information() const override;

        //	IWeb methods
        // -------------------------------------------------------------------------------------------------------
        virtual void Inbound(Web::Request& request) override;
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request) override;

    private:
        void StateChange(PluginHost::IShell* plugin);
        void StateChange(Exchange::IResourceCenter::hardware_state state);

        void Attached(Exchange::IComposition::IClient* client);
        void Detached(Exchange::IComposition::IClient* client);

        void Clients(Core::JSON::ArrayType<Core::JSON::String>& clients) const;
        void Kill(const string& client) const;
        void Focus(const string& client) const;
        void Opacity(const string& client, const uint32 value) const;
        void Visible(const string& client, const bool visible) const;
        void Geometry(const string& client, const uint32 x, const uint32 y, const uint32 width, const uint32 height) const;
        void Top(const string& client) const;
        void Input(const string& client) const;

    private:
        mutable Core::CriticalSection _adminLock;
        uint8 _skipURL;
        Core::Sink<Notification> _notification;
        Exchange::IResourceCenter* _resourceCenter;
        Exchange::IComposition* _composition;
        PluginHost::ISystemInfo* _systemInfo;
        PluginHost::IShell* _service;
        string _callsign;
        uint32 _pid;
        std::map<string, Exchange::IComposition::IClient*> _clients;
    };
}
}

#endif // __PLUGIN_COMPOSITOR_H
