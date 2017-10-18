#ifndef __BROWSER_H
#define __BROWSER_H

#include "Module.h"
#include <interfaces/IBrowser.h>
#include <interfaces/IMemory.h>
#include <interfaces/IComposition.h>

namespace WPEFramework {
namespace Plugin {

    class WebKitBrowser : public PluginHost::IPlugin, public PluginHost::IWeb {
    private:
        WebKitBrowser(const WebKitBrowser&);
        WebKitBrowser& operator=(const WebKitBrowser&);

        class Notification : 
            protected PluginHost::IPlugin::INotification,
            protected PluginHost::ISystemInfo::INotification,
            protected RPC::IRemoteProcess::INotification,
            public PluginHost::IStateControl::INotification,
            public Exchange::IBrowser::INotification {

        private:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
            explicit Notification(WebKitBrowser* parent)
                : _parent(*parent)
                , _systemInfoCallsign()
                , _compositionCallsign()
                , _clientName()
                , _service(nullptr)
                , _systemInfo(nullptr)
                , _composition(nullptr)
                , _hidden(true)
            {
                ASSERT(parent != nullptr);
            }
            ~Notification()
            {
            }

        public:
            inline bool WaitForSystem() const {

                return (_systemInfoCallsign.empty() == false);
            }
            inline bool Hidden () const {
                return (_hidden);
            }
            inline void Attach (PluginHost::IShell* service, const string& systemInfo, const string& clientName)
            {
                _adminLock.Lock();

                ASSERT (service != nullptr);
                ASSERT (_service == nullptr);
                ASSERT (_systemInfo == nullptr);
                ASSERT (_composition == nullptr);

                _systemInfoCallsign = systemInfo;
                _clientName = clientName;
                _hidden = true;

                _service = service;
                _service->AddRef();

                _service->Register (static_cast<RPC::IRemoteProcess::INotification*>(this));
                _service->Register (static_cast<PluginHost::IPlugin::INotification*>(this));

                _adminLock.Unlock();
            }
            inline void Detach()
            {
	        ASSERT (_service != nullptr);

                _adminLock.Lock();

                if (_service != nullptr) {

                    _service->Unregister (static_cast<PluginHost::IPlugin::INotification*>(this));
                    _service->Unregister (static_cast<RPC::IRemoteProcess::INotification*>(this));

                    // If we are subscribed, start unsibscribing from root sources
                    if (_systemInfo != nullptr) 
                    {
                        _systemInfo->Unregister(this);
                        _systemInfo->Release();
                        _systemInfo = nullptr;
                    }

                    // If we are subscribed, start unsibscribing from root sources
                    if (_composition != nullptr) 
                    {
                        _composition->Release();
                        _composition = nullptr;
                    }

                    _service->Release();
                    _service = nullptr;

                    _systemInfoCallsign.clear();
                }

                _adminLock.Unlock();
            }
            virtual void LoadFinished(const string& URL);
            virtual void URLChanged(const string& URL);
            virtual void Hidden(const bool hidden);
            virtual void StateChange(const PluginHost::IStateControl::state state);
            virtual void Activated(RPC::IRemoteProcess* process);
            virtual void Deactivated(RPC::IRemoteProcess* process);

            BEGIN_INTERFACE_MAP(Notification)
                INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
                INTERFACE_ENTRY(PluginHost::ISystemInfo::INotification)
                INTERFACE_ENTRY(Exchange::IBrowser::INotification)
                INTERFACE_ENTRY(PluginHost::IStateControl::INotification)
                INTERFACE_ENTRY(RPC::IRemoteProcess::INotification)
            END_INTERFACE_MAP

        private:
            inline void Focus (){

                if ( (_state == PluginHost::IStateControl::RESUMED) && (_composition != nullptr) ) {

                    Exchange::IComposition::IClient* client (_composition->Client(_clientName));

                    if (client != nullptr) {
                        client->Focus();
                        client->Release();
                    }
                }
            }
            virtual void StateChange(PluginHost::IShell* plugin) 
            {
                const string callsign (plugin->Callsign());
                PluginHost::IShell::state state (plugin->State());

                _adminLock.Lock();

                // Check the SystemState, if still required, for its current state..
                if ( (_systemInfoCallsign.empty() == false) && (callsign == _systemInfoCallsign) ) {

                    if (state == PluginHost::IShell::ACTIVATED) {

                        ASSERT (_systemInfo == nullptr);
                        _systemInfo = plugin->QueryInterface<PluginHost::ISystemInfo>();

                        if (_systemInfo != nullptr) {
                            _systemInfo->Register(this);
                        }
                    }
                    else if ( (state == PluginHost::IShell::DEACTIVATION) && (_systemInfo != nullptr) ) {
                        _systemInfo->Unregister(this);
                        _systemInfo->Release();
                        _systemInfo = nullptr;
                    }
                }
                else if (_compositionCallsign.empty() == true) {

                    if (state == PluginHost::IShell::ACTIVATED) {

                        _composition = plugin->QueryInterface<Exchange::IComposition>();

                        if (_composition != nullptr) {
                            _compositionCallsign = callsign;
                            Focus();
                        }
                    }
                }
                else if (_compositionCallsign == callsign) {

                    if (state == PluginHost::IShell::ACTIVATED) {

                        ASSERT (_composition == nullptr);

                        _composition = plugin->QueryInterface<Exchange::IComposition>();

                        ASSERT (_composition != nullptr);

                        Focus();
                    }
                    else if ( (state == PluginHost::IShell::DEACTIVATION) && (_composition == nullptr) ) {

                        _composition->Release();
                        _composition= nullptr;
                    }
                }

                _adminLock.Unlock();
            }
            void Updated()
            {
                _adminLock.Lock();

                ASSERT (_systemInfo != nullptr);

                if ( (_systemInfo != nullptr) && (_systemInfo->SystemState() >= PluginHost::ISystemInfo::OPERATIONAL) ) {

                    _parent.Go();

                    _systemInfoCallsign.clear();
                    _systemInfo->Unregister (this);
                    _systemInfo->Release();
                    _systemInfo = nullptr;
                }

                _adminLock.Unlock();
            }

        private:
            WebKitBrowser& _parent;
            Core::CriticalSection _adminLock;
            string _systemInfoCallsign;
            string _compositionCallsign;
            string _clientName;
            PluginHost::IShell* _service;
            PluginHost::ISystemInfo* _systemInfo;
            Exchange::IComposition* _composition;
            PluginHost::IStateControl::state _state;
            bool _hidden;
        };

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

        public:
            Config()
                : Core::JSON::Container()
                , OutOfProcess(true)
                , System(_T("Controller"))
            {
                Add(_T("outofprocess"), &OutOfProcess);
                Add(_T("system"), &System);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::Boolean OutOfProcess;
            Core::JSON::String System;
        };

    public:
        class Data : public Core::JSON::Container {
        private:
            Data(const Data&) = delete;
            Data& operator=(const Data&) = delete;

        public:
            Data()
                : Core::JSON::Container()
                , URL()
                , FPS()
                , Suspended(false)
                , Hidden(false)
            {
                Add(_T("url"), &URL);
                Add(_T("fps"), &FPS);
                Add(_T("suspended"), &Suspended);
                Add(_T("hidden"), &Hidden);
            }
            ~Data()
            {
            }

        public:
            Core::JSON::String URL;
            Core::JSON::DecUInt32 FPS;
            Core::JSON::Boolean Suspended;
            Core::JSON::Boolean Hidden;
        };

    public:
        WebKitBrowser()
            : _skipURL(0)
            , _service(nullptr)
            , _browser(nullptr)
            , _memory(nullptr)
            , _notification(this)
        {
        }
        virtual ~WebKitBrowser()
        {
            TRACE_L1("Destructor WebKitBrowser.%d", __LINE__);
        }

    public:
        BEGIN_INTERFACE_MAP(WebKitBrowser)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IWeb)
            INTERFACE_AGGREGATE(PluginHost::IStateControl, _browser)
            INTERFACE_AGGREGATE(Exchange::IBrowser, _browser)
            INTERFACE_AGGREGATE(Exchange::IMemory, _memory)
        END_INTERFACE_MAP

    public:
        //  IPlugin methods
        // -------------------------------------------------------------------------------------------------------

        // First time initialization. Whenever a plugin is loaded, it is offered a Service object with relevant
        // information and services for this particular plugin. The Service object contains configuration information that
        // can be used to initialize the plugin correctly. If Initialization succeeds, return nothing (empty string)
        // If there is an error, return a string describing the issue why the initialisation failed.
        // The Service object is *NOT* reference counted, lifetime ends if the plugin is deactivated.
        // The lifetime of the Service object is guaranteed till the deinitialize method is called.
        virtual const string Initialize(PluginHost::IShell* service);

        // The plugin is unloaded from WPEFramework. This is call allows the module to notify clients
        // or to persist information if needed. After this call the plugin will unlink from the service path
        // and be deactivated. The Service object is the same as passed in during the Initialize.
        // After theis call, the lifetime of the Service object ends.
        virtual void Deinitialize(PluginHost::IShell* service);

        // Returns an interface to a JSON struct that can be used to return specific metadata information with respect
        // to this plugin. This Metadata can be used by the MetData plugin to publish this information to the ouside world.
        virtual string Information() const;

        //  IWeb methods
        // -------------------------------------------------------------------------------------------------------
        virtual void Inbound(Web::Request& request);
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

    private:
        void Deactivated(RPC::IRemoteProcess* process);
        void Go();

    private:
        uint8 _skipURL;
        uint32 _pid;
        string _webPath;
        PluginHost::IShell* _service;
        Exchange::IBrowser* _browser;
        Exchange::IMemory* _memory;
        Core::Sink<Notification> _notification;
    };
}
}

#endif // __BROWSER_H
