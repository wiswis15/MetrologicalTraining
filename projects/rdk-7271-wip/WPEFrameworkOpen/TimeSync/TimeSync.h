#ifndef TIMESYNC_H
#define TIMESYNC_H

#include "Module.h"
#include <interfaces/ITimeSync.h>

namespace WPEFramework {
namespace Plugin {

    class TimeSync : public PluginHost::IPlugin, public PluginHost::IWeb {
    public:
        class Data : public Core::JSON::Container {
        public:
            Data(Data const& other) = delete;
            Data& operator=(Data const& other) = delete;

            Data()
                : Core::JSON::Container()
                , IsTimeSynced()
                , TimeSource()
                , SyncTime()
            {
                Add(_T("synced"), &IsTimeSynced);
                Add(_T("source"), &TimeSource);
                Add(_T("time"), &SyncTime);
            }

            virtual ~Data()
            {
            }

        public:
            Core::JSON::Boolean IsTimeSynced;
            Core::JSON::String TimeSource;
            Core::JSON::String SyncTime;
        };

    private:
        class Notification :
            protected PluginHost::IPlugin::INotification,
			protected PluginHost::ISystemInfo::INotification,
			protected Exchange::ITimeSync::INotification {
        private:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
            explicit Notification(TimeSync* parent)
                : _adminLock()
                , _parent(*parent)
                , _callsign()
                , _systemInfo(nullptr)
                , _service(nullptr)
                , _client(nullptr)
                , _registered(false) {

                ASSERT(parent != nullptr);
            }
            ~Notification() {
            }

        public:
            void Initialize(PluginHost::IShell* service, Exchange::ITimeSync* client, const string& callsign) {

                _callsign = callsign;

                ASSERT(_service == nullptr);
                ASSERT(service != nullptr);
                ASSERT(_client == nullptr);
                ASSERT(client != nullptr);

                _client = client;
                _client->AddRef();

                _service = service;
                _service->AddRef();

                _service->Register(this);
                _client->Register(this);
            }
            void Deinitialize() {

                ASSERT(_service != nullptr);
                ASSERT(_client != nullptr);

                _client->Unregister(this);
                _client->Release();
                _client = nullptr;

                // First un-register the plugin notifications, so the _systemInfo
                // con not change if we un-register that one if it still exists..
                _service->Unregister(this);
                _service->Release();
                _service = nullptr;

                // Is there a _systemInfo, un-register it, it can not change,
                // StateChange can not be called due to the un-register above.
                if (_systemInfo != nullptr) {

                    if (_registered == true) {

                        _systemInfo->Unregister(this);
                    }

                    _systemInfo->Release();
                    _systemInfo = nullptr;
                }

            }
            virtual void StateChange(PluginHost::IShell* plugin) {
                // On activation subscribe, on deactivation un-subscribe
                if (plugin->Callsign() == _callsign) {

                    if (plugin->State() == PluginHost::IShell::ACTIVATED) {

                        ASSERT(_systemInfo == nullptr);

                        _systemInfo = plugin->QueryInterface<PluginHost::ISystemInfo>();

                        if (_systemInfo != nullptr) {

                            if (_systemInfo->NetworkType() == PluginHost::ISystemInfo::UNKNOWN) {

                                _registered = true;
                                _systemInfo->Register(this);
                            }
                            else {

                                uint64 timeStamp = _client->SyncTime();

                                if (timeStamp != 0) {

                                    _systemInfo->TimeSync(timeStamp);
                                }
                                else {
                                    TRACE_L1("Timesync::Notification::StateChange %s", "Synchronize");
                                    _client->Synchronize();
                                }
                            }
                        }
                    }
                    else if ( (_systemInfo != nullptr) && (plugin->State() == PluginHost::IShell::DEACTIVATION) ) {

                        ASSERT(_systemInfo != nullptr);

                        if (_registered == true) {

                            _registered = false;
                            _systemInfo->Unregister(this);
                        }
                        _systemInfo->Release();

                        _adminLock.Lock();
                        _systemInfo = nullptr;
                        _adminLock.Unlock();
                    }
                }
            }
            virtual void Updated() {
                ASSERT(_systemInfo != nullptr);

                if ((_systemInfo->NetworkType() == PluginHost::ISystemInfo::IPV4) ||
                    (_systemInfo->NetworkType() == PluginHost::ISystemInfo::IPV6)) {

                    _registered = false;
                    _systemInfo->Unregister(this);

                    // Safe to start a time sync..
                    _client->Synchronize();
                }
            }
            virtual void Completed() {
                uint64 timeTicks = _client->SyncTime();

                if (timeTicks != 0) {
                    _parent.SyncedTime(timeTicks);
                    _adminLock.Lock();
                    if (_systemInfo != nullptr) {
                        _systemInfo->TimeSync(timeTicks);
                    }
                    _adminLock.Unlock();
                }
            }

            BEGIN_INTERFACE_MAP(Notification)
                INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
                INTERFACE_ENTRY(PluginHost::ISystemInfo::INotification)
                INTERFACE_ENTRY(Exchange::ITimeSync::INotification)
            END_INTERFACE_MAP

        private:
            Core::CriticalSection _adminLock;
            TimeSync& _parent;
            string _callsign;
            PluginHost::ISystemInfo* _systemInfo;
            PluginHost::IShell* _service;
            Exchange::ITimeSync* _client;
            bool _registered;
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
				, Sources()
				, Periodicity(0)
            {
                Add(_T("system"), &System);
                Add(_T("interval"), &Interval);
                Add(_T("retries"), &Retries);
                Add(_T("sources"), &Sources);
                Add(_T("periodicity"), &Periodicity);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::String System;
            Core::JSON::DecUInt16 Interval;
            Core::JSON::DecUInt8 Retries;
            Core::JSON::ArrayType<Core::JSON::String> Sources;
            Core::JSON::DecUInt16 Periodicity;
        };

        class PeriodicSync : public Core::IDispatchType<void> {
        private:
            PeriodicSync() = delete;
            PeriodicSync(const PeriodicSync&) = delete;
            PeriodicSync& operator=(const PeriodicSync&) = delete;

        public:
            PeriodicSync(Exchange::ITimeSync* timeSync)
                : _timeSync(timeSync)
            {
                ASSERT(timeSync != nullptr);

                _timeSync = timeSync;
                _timeSync->AddRef();
            }
            ~PeriodicSync()
            {
                _timeSync->Release();
            }

        public:
            virtual void Dispatch() override
            {
                _timeSync->Synchronize();
            }

        private:
            Exchange::ITimeSync* _timeSync;
        };

    private:
        TimeSync(const TimeSync&) = delete;
        TimeSync& operator=(const TimeSync&) = delete;

    public:
        TimeSync();
        virtual ~TimeSync();

        // Build QueryInterface implementation, specifying all possible interfaces to be returned.
        BEGIN_INTERFACE_MAP(TimeSync)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        INTERFACE_AGGREGATE(Exchange::ITimeSync, _client)
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
        void SyncedTime(const uint64 timeTicks);

    private:
        uint16 _skipURL;
        uint32 _periodicity;
        Exchange::ITimeSync* _client;
        Core::ProxyType<Core::IDispatchType<void> > _activity;
        Core::Sink<Notification> _sink;
    };

} // namespace Plugin
} // namespace WPEFramework

#endif // TIMESYNC_H
