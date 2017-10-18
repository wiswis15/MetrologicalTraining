#ifndef __CONTROLLER_SYSTEMINFO_H__
#define __CONTROLLER_SYSTEMINFO_H__ 

#include "Module.h"
#include "Controller.h"

namespace WPEFramework {
	namespace Plugin {

    class SystemInfo : public PluginHost::ISystemInfo {
    private:
	SystemInfo() = delete;
	SystemInfo(const SystemInfo&) = delete;
	SystemInfo& operator= (const SystemInfo&) = delete;

			class Job : public Core::IDispatchType<void> {
			private:
				Job() = delete;
				Job(const Job&) = delete;
				Job& operator=(const Job&) = delete;

			public:
				Job(SystemInfo* parent) : _parent(*parent)
				{
					ASSERT (parent != nullptr);
				}
				virtual ~Job()
				{
				}

			public:
				virtual void Dispatch()
				{
				    _parent.Notify();
				}

			private:
				SystemInfo& _parent;
			};

    public:
	SystemInfo(Controller::Config* config, const string& accessor);
	virtual ~SystemInfo();

    public:
	virtual void Register(PluginHost::ISystemInfo::INotification* notification) override;
	virtual void Unregister(PluginHost::ISystemInfo::INotification* notification) override;

        // Hardware status 
        virtual PluginHost::ISystemInfo::system_state SystemState () const override;
        virtual PluginHost::ISystemInfo::system_state SystemState (const PluginHost::ISystemInfo::system_state newState) override;

        // Network information
        virtual string PublicIPAddress() const override;
        virtual PluginHost::ISystemInfo::network_type NetworkType() const override;
        virtual void PublicIPAddress(const string& ip) override;

        // Location information
        virtual string TimeZone() const override;
        virtual string Country() const override;
        virtual string Region() const override;
        virtual string City() const override;
        virtual void Location(const string& timeZone, const string& country, const string& region, const string& city) override;

        // Device information
        virtual uint8 Identifier(const uint8 length, uint8 buffer[]) const override;
        virtual void Identifier(const uint8 length, const uint8 buffer[]) override;

        // Time synchronisation reporting.
        virtual uint64 TimeSync() const override;
        virtual void TimeSync(const uint64 ticks) override;

       BEGIN_INTERFACE_MAP(SystemInfo)
            INTERFACE_ENTRY(PluginHost::ISystemInfo)
        END_INTERFACE_MAP

    private:
        inline void Update()
        {
            PluginHost::WorkerPool::Instance().Submit(_job);
        }
        void Notify();

    private:
        mutable Core::CriticalSection _adminLock;
        std::list<PluginHost::ISystemInfo::INotification*> _notificationClients;
        Core::ProxyType<Core::IDispatchType<void> > _job;
        string _ipAddress;
        string _timeZone;
        string _country;
        string _region;
        string _city;
        uint8* _identifier;
        uint64 _timeSync;
        ISystemInfo::system_state _systemState;
    };

} }  // namespace Plugin

#endif
