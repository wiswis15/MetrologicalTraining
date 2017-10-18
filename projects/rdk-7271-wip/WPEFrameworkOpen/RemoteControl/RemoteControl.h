#ifndef __PLUGINWEBPROXY_H
#define __PLUGINWEBPROXY_H

#include "Module.h"
#include "RemoteAdministrator.h"
#include <interfaces/IKeyHandler.h>

namespace WPEFramework {
namespace Plugin {

    class RemoteControl : public PluginHost::IPlugin, public PluginHost::IWeb, public Exchange::IKeyHandler {
    private:
        RemoteControl(const RemoteControl&);
        RemoteControl& operator=(const RemoteControl&);

    private:
        class Notification :
            protected PluginHost::IPlugin::INotification,
            protected PluginHost::ISystemInfo::INotification {

        private:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
            explicit Notification(Exchange::IKeyHandler* handler)
                : _adminLock()
                , _handler(handler)
                , _callsign()
                , _systemInfo(nullptr)
                , _registered(false) {

                ASSERT(handler != nullptr);
            }
            ~Notification() {
            }

        public:
            void Initialize(PluginHost::IShell* service, const string& callsign) {
                TRACE_L1("RemoteControl::Notification::Initialize %s", callsign.c_str());

                if (callsign.empty() == true) {

                    Remotes::RemoteAdministrator::Instance().Callback(_handler);
                    _registered = true;
                }
                else {

                    _callsign = callsign;
                    service->Register(this);
                }
            }
            void Deinitialize(PluginHost::IShell* service) {

                if (_callsign.empty() == false) {

                    Remotes::RemoteAdministrator::Instance().Callback(nullptr);
                    _registered = false;
                }
                else {
                    // First un-register the plugin notifications, so the _systemInfo
                    // can not change if we un-register that one if it still exists..
                    service->Unregister(this);

                    _adminLock.Lock();

                    // Is there a _systemInfo, un-register it, it can not change,
                    // StateChange can not be called due to the un-register above.
                    if (_systemInfo != nullptr) {

                        if (_registered == true) {

                            _systemInfo->Unregister(this);
                        }

                        _systemInfo->Release();
                        _systemInfo = nullptr;
                    }
                    else if (_registered == true) {
            
                        Remotes::RemoteAdministrator::Instance().Callback(nullptr);
                        _registered = false;
                    }
                
                    _adminLock.Unlock();
                    _callsign.clear();
                }
            }
            virtual void StateChange(PluginHost::IShell* plugin) {

                // On activation subscribe, on deactivation un-subscribe
                if (plugin->Callsign() == _callsign) {

                    // If the plugin is in a mode change, we do not expect the Update to be called,
                    // however, our "deinitialize" might be called so lock this as well.

                    _adminLock.Lock();

                    if (plugin->State() == PluginHost::IShell::ACTIVATED) {

                        ASSERT(_systemInfo == nullptr);

                        _systemInfo = plugin->QueryInterface<PluginHost::ISystemInfo>();

                        if (_systemInfo != nullptr) {

                            if (_systemInfo->SystemState() < PluginHost::ISystemInfo::OPERATIONAL) {

                                _systemInfo->Register(this);
                            }
                            else {

                                _systemInfo->Release();
                                _systemInfo = nullptr;

                                _registered = true;
                                Remotes::RemoteAdministrator::Instance().Callback(_handler);
                            }
                        }
                    }
                    else if ( (_systemInfo != nullptr) && (plugin->State() == PluginHost::IShell::DEACTIVATION) ) {

                        _systemInfo->Unregister(this);
                        _systemInfo->Release();
                        _systemInfo = nullptr;
                    }

                    _adminLock.Unlock();
                }
            }
            virtual void Updated() {

                _adminLock.Lock();

                if ( (_systemInfo != nullptr) && (_systemInfo->SystemState() >= PluginHost::ISystemInfo::OPERATIONAL) ) {

                    _systemInfo->Unregister(this);
                    _systemInfo->Release();
                    _systemInfo = nullptr;

                    // Hardware is ready, please continue...
                    _registered = true;
                    Remotes::RemoteAdministrator::Instance().Callback(_handler);
                }

                _adminLock.Unlock();
            }

            BEGIN_INTERFACE_MAP(Notification)
                INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
                INTERFACE_ENTRY(PluginHost::ISystemInfo::INotification)
            END_INTERFACE_MAP

        private:
            Core::CriticalSection _adminLock;
            Exchange::IKeyHandler* _handler;
            string _callsign;
            PluginHost::ISystemInfo* _systemInfo;
            bool _registered;
        };

    public:
        class Config : public Core::JSON::Container {
        private:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            class Mapping : public Core::JSON::Container {
            private:
                Mapping& operator=(const Mapping&) = delete;

            public:
                Mapping()
                    : Core::JSON::Container()
                    , Device()
                    , MapFile()
                    , PassOn(false)
                    , CodeMask(static_cast<uint32>(~0))
                {
                    Add(_T("device"), &Device);
                    Add(_T("mapfile"), &MapFile);
                    Add(_T("passon"), &PassOn);
                    Add(_T("codemask"), &CodeMask);
                }
                Mapping(const Mapping& copy)
                    : Core::JSON::Container()
                    , Device(copy.Device)
                    , MapFile(copy.MapFile)
                    , PassOn(copy.PassOn)
                    , CodeMask(copy.CodeMask)
                {
                    Add(_T("device"), &Device);
                    Add(_T("mapfile"), &MapFile);
                    Add(_T("passon"), &PassOn);
                    Add(_T("codemask"), &CodeMask);
                }
                ~Mapping()
                {
                }

                Core::JSON::String Device;
                Core::JSON::String MapFile;
                Core::JSON::Boolean PassOn;
                Core::JSON::HexUInt32 CodeMask;
            };

        public:
            Config()
                : Core::JSON::Container()
                , MapFile()
                , System()
                , PassOn(false)
                , RepeatStart(500)
                , RepeatInterval(100)
                , Specific()
            {
                Add(_T("mapfile"), &MapFile);
                Add(_T("system"), &System);
                Add(_T("passon"), &PassOn);
                Add(_T("repeatstart"), &RepeatStart);
                Add(_T("repeatinterval"), &RepeatInterval);
                Add(_T("specific"), &Specific);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::String MapFile;
            Core::JSON::String System;
            Core::JSON::Boolean PassOn;
            Core::JSON::DecUInt16 RepeatStart;
            Core::JSON::DecUInt16 RepeatInterval;
            Core::JSON::ArrayType<Mapping> Specific;
        };

        class Data : public Core::JSON::Container {
        public:
            class DeviceInfo : public Core::JSON::Container {
            public:
                DeviceInfo()
                    : Core::JSON::Container()
                    , Name()
                {
                    Add(_T("name"), &Name);
                }
                virtual ~DeviceInfo()
                {
                }

                DeviceInfo(const string& name)
                        : Core::JSON::Container()
                        , Name(name)
                {
                    Add(_T("name"), &Name);
                }
                DeviceInfo(const DeviceInfo& copy)
                        : Core::JSON::Container()
                        , Name(copy.Name)
                {

                    Add(_T("name"), &Name);
                }
                DeviceInfo& operator=(const DeviceInfo& RHS)
                {

                    Name = RHS.Name;

                    return *this;
                }
            public:
                Core::JSON::String Name;
            };

        private:
            Data(const Data&) = delete;
            Data& operator=(const Data&) = delete;

        public:
            Data()
                : Core::JSON::Container()
                , Devices()
            {
                Add(_T("devices"), &Devices);
            }

            virtual ~Data()
            {
            }

        public:
            Core::JSON::ArrayType<DeviceInfo> Devices;
        };

    public:
        RemoteControl();
        virtual ~RemoteControl();

        BEGIN_INTERFACE_MAP(RemoteControl)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        INTERFACE_ENTRY(Exchange::IKeyHandler)
        END_INTERFACE_MAP

    public:
        //	IPlugin methods
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

        //      IWeb methods
        // -------------------------------------------------------------------------------------------------------
        // Whenever a request is received, it might carry some additional data in the body. This method allows
        // the plugin to attach a deserializable data object (ref counted) to be loaded with any potential found
        // in the body of the request.
        virtual void Inbound(Web::Request& request);

        // If everything is received correctly, the request is passed on to us, through a thread from the thread pool, to
        // do our thing and to return the result in the response object. Here the actual specific module work,
        // based on a a request is handled.
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

        //      IKeyHandler methods
        // -------------------------------------------------------------------------------------------------------
        // Whnever a key is pressed or release, let this plugin now, it will take the proper arrangements and timings
        // to announce this key event to the linux system. Repeat event is triggered by the watchdog implementation
        // in this plugin. No need to signal this.
        virtual uint32 KeyEvent(const bool pressed, const uint32 code, const string& table);

    private:
        Core::ProxyType<Web::Response> GetMethod(Core::TextSegmentIterator& index) const;
        Core::ProxyType<Web::Response> PutMethod(Core::TextSegmentIterator& index, const Web::Request& request);
        Core::ProxyType<Web::Response> DeleteMethod(Core::TextSegmentIterator& index);
        Core::ProxyType<Web::Response> PostMethod(Core::TextSegmentIterator& index, const Web::Request& request);
        const string FindDevice(Core::TextSegmentIterator& index) const;

    private:
        uint32 _skipURL;
        PluginHost::VirtualInput* _inputHandler;
        Core::Sink<Notification> _sink;
        string _persistentPath;
    };
}
}

#endif // __PLUGINWEBPROXY_H
