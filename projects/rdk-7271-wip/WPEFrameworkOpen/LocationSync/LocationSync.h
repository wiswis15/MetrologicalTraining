#ifndef LOCATIONSYNC_LOCATIONSYNC_H
#define LOCATIONSYNC_LOCATIONSYNC_H

#include "Module.h"
#include "LocationService.h"

namespace WPEFramework {
namespace Plugin {

    class LocationSync : public PluginHost::IPlugin, public PluginHost::IWeb {
    public:
        class Data : public Core::JSON::Container {
        public:
            Data(Data const& other) = delete;
            Data& operator=(Data const& other) = delete;

            Data()
                : Core::JSON::Container()
                , PublicIp()
                , TimeZone()
                , Region()
                , Country()
                , City()
            {
                Add(_T("ip"), &PublicIp);
                Add(_T("timezone"), &TimeZone);
                Add(_T("region"), &Region);
                Add(_T("country"), &Region);
                Add(_T("city"), &Region);
            }

            virtual ~Data()
            {
            }

        public:
            Core::JSON::String PublicIp;
            Core::JSON::String TimeZone;
            Core::JSON::String Region;
            Core::JSON::String Country;
            Core::JSON::String City;
        };

    private:
        class Notification : protected PluginHost::IPlugin::INotification,
							 protected Core::IDispatchType<void> {
        private:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
			#ifdef __WIN32__ 
			#pragma warning( disable : 4355 )
			#endif
            explicit Notification(LocationSync* parent)
                : _parent(*parent)
                , _adminLock()
                , _callsign()
                , _systemInfo(nullptr)
                , _service(nullptr)
                , _locator(this)
            {

                ASSERT(parent != nullptr);
            }
			#ifdef __WIN32__ 
			#pragma warning( default : 4355 )
			#endif
			~Notification()
            {
            }

        public:
            void Initialize(PluginHost::IShell* service, const string& callsign)
            {
                TRACE_L1("LocationSync::Notification::Initialize %s", callsign.c_str());
                _callsign = callsign;

                ASSERT(_service == nullptr);
                ASSERT(service != nullptr);

                _service = service;
                _service->AddRef();
                _service->Register(this);
            }
            void Deinitialize()
            {
                _adminLock.Lock();

                if (_service != nullptr)
                {
                    // First un-register the plugin notifications, so the _systemInfo
                    // can not change if we un-register that one if it still exists..
                    _service->Unregister(this);
                    _service->Release();
                    _service = nullptr;


                    // Is there a _systemInfo, un-register it, it can not change,
                    // StateChange can not be called due to the un-register above.
                    if (_systemInfo != nullptr) {

                        _systemInfo->Release();
                        _systemInfo = nullptr;
                    }
                }

                _adminLock.Unlock();
            }
            uint32 Probe(const string& remoteNode, const uint32 retries, const uint32 retryTimeSpan)
            {
                return (_locator.Probe(remoteNode, retries, retryTimeSpan));
            }
            inline const string& PublicIPAddress() const
            {
                return (_locator.PublicIPAddress());
            }
            inline const string& TimeZone() const
            {
                return (_locator.TimeZone());
            }
            inline const string& Country() const
            {
                return (_locator.Country());
            }
            inline const string& Region() const
            {
                return (_locator.Region());
            }
            inline const string& City() const
            {
                return (_locator.City());
            }
            virtual void StateChange(PluginHost::IShell* plugin)
            {
                // On activation subscribe, on deactivation un-subscribe
                if (plugin->Callsign() == _callsign) {

                    _adminLock.Lock();

                    if (plugin->State() == PluginHost::IShell::ACTIVATED) {

                        ASSERT(_systemInfo == nullptr);

                        _systemInfo = plugin->QueryInterface<PluginHost::ISystemInfo>();

                        if (_systemInfo != nullptr) {

                            // If we got data, fill it in.
                            SetInfo();
                        }
                    }
                    else if ((_systemInfo != nullptr) && (plugin->State() == PluginHost::IShell::DEACTIVATION)) {

                        _systemInfo->Release();
                        _systemInfo = nullptr;
                    }

                    _adminLock.Unlock();
                }
            }
            virtual void Dispatch()
            {
                _adminLock.Lock();
                if (_systemInfo != nullptr) {
                    SetInfo();
                }
                _adminLock.Unlock();

                _parent.SyncedLocation();
            }
            void SetInfo()
            {
                const string& ip(_locator.PublicIPAddress());

                if (ip.empty() == false) {

                    ASSERT(_systemInfo != nullptr);

                    _systemInfo->PublicIPAddress(_locator.PublicIPAddress());
                    _systemInfo->Location(_locator.TimeZone(), _locator.Country(), _locator.Region(), _locator.City());
                }
            }

            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
            END_INTERFACE_MAP

        private:
            LocationSync& _parent;
            Core::CriticalSection _adminLock;
            string _callsign;
            PluginHost::ISystemInfo* _systemInfo;
            PluginHost::IShell* _service;
            LocationService _locator;
        };

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

        public:
            Config() 
				: System("Controller")
				, Interval(30)
				, Retries(8)
				, Source()
            {
                Add(_T("system"), &System);
                Add(_T("interval"), &Interval);
                Add(_T("retries"), &Retries);
                Add(_T("source"), &Source);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::String System;
            Core::JSON::DecUInt16 Interval;
            Core::JSON::DecUInt8 Retries;
            Core::JSON::String Source;
        };

    private:
        LocationSync(const LocationSync&) = delete;
        LocationSync& operator=(const LocationSync&) = delete;

    public:
        LocationSync();
        virtual ~LocationSync();

        // Build QueryInterface implementation, specifying all possible interfaces to be returned.
        BEGIN_INTERFACE_MAP(LocationSync)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        virtual const string Initialize(PluginHost::IShell* service) override;
        virtual void Deinitialize(PluginHost::IShell* service) override;
        virtual string Information() const override;

        //   IWeb methods
        // -------------------------------------------------------------------------------------------------------
        virtual void Inbound(Web::Request& request) override;
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request) override;

    private:
        void SyncedLocation();

    private:
        uint16 _skipURL;
        string _source;
        Core::Sink<Notification> _sink;
    };

} // namespace Plugin
} // namespace WPEFramework

#endif // LOCATIONSYNC_LOCATIONSYNC_H
