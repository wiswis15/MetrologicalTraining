#include "SystemInfo.h"

namespace WPEFramework {
    namespace Plugin {

        typedef Core::IteratorType< std::list<PluginHost::ISystemInfo::INotification*>, PluginHost::ISystemInfo::INotification*> ClientIterator;

        static uint8* SetIdentifier(const uint8 length, const uint8 buffer[])
        {
            uint8* copy = new uint8[length + 2];

            ASSERT(copy != nullptr);

            ::memcpy(&(copy[1]), buffer, length);

            copy[0] = length;
            copy[length + 1] = '\0';

            return (copy);
        }

        static void RecursiveList (ClientIterator& index)
        {
            PluginHost::ISystemInfo::INotification* callee (*index);

            ASSERT (callee != nullptr);

            if (callee != nullptr) 
            {
                callee->AddRef();

                if (index.Next() == true) 
                {
                    RecursiveList(index);
                }

                callee->Updated();
                callee->Release();
            }
        }

		#ifdef __WIN32__ 
		#pragma warning( disable : 4355 )
		#endif
        SystemInfo::SystemInfo(Controller::Config* config, const string& accessor)
            : _adminLock()
            , _notificationClients()
            , _job(Core::ProxyType<SystemInfo::Job>::Create(this))
            , _ipAddress()
            , _timeZone()
            , _country()
            , _region()
            , _city()
            , _identifier(nullptr)
            , _timeSync(0)
            , _systemState(ISystemInfo::POWERON)
        {
            ASSERT (config != nullptr);

            if ((config->Location.Value() == Controller::Config::Internal)) {

                // Location is also responsible for setting network info, if it is INTERNAL assume Network is oke.
                // Determine the type of network as good as you can..
                Core::URL mySource(accessor);

                _ipAddress = mySource.Host().Value().Text();

                // We can not make the other fields up, so leave them blank..
            }

            if (config->Identifier.Value() == Controller::Config::Internal) {
                const uint8* id(Core::SystemInfo::Instance().RawDeviceId());
                _identifier = SetIdentifier(id[0], &id[1]);
            }

            if ((config->Time.Value() == Controller::Config::Internal)) {
                _timeSync = Core::Time::Now().Ticks();
            }

            if ((config->Hardware.Value() == Controller::Config::Internal)) {
                _systemState = ISystemInfo::OPERATIONAL;
            }
        }
		#ifdef __WIN32__ 
		#pragma warning( default : 4355 )
		#endif

        /* virtual */ SystemInfo::~SystemInfo()
        {
        }

        void SystemInfo::Register(PluginHost::ISystemInfo::INotification* notification)
        {
            _adminLock.Lock();

            ASSERT (std::find(_notificationClients.begin(), _notificationClients.end(), notification) == _notificationClients.end());

            _notificationClients.push_back(notification);

            // Give the registering sink a chance to evaluate the current info before one actually changes.
            notification->Updated();

            _adminLock.Unlock();
        }
		
        void SystemInfo::Unregister(PluginHost::ISystemInfo::INotification* notification)
        {
            _adminLock.Lock();

            std::list<PluginHost::ISystemInfo::INotification*>::iterator index (std::find(_notificationClients.begin(), _notificationClients.end(), notification));

            ASSERT(index != _notificationClients.end());

            if (index != _notificationClients.end())
            {
                _notificationClients.erase(index);
            }

            _adminLock.Unlock();
        }

        void SystemInfo::Notify()
        {
            _adminLock.Lock();

            ClientIterator index(_notificationClients);

            if (index.Next() == true) 
            {
                RecursiveList(index);
            }

            _adminLock.Unlock();
        }

        // Network information
        /* virtual */ string SystemInfo::PublicIPAddress() const
        {
            return (_ipAddress);
        }

        /* virtual */ PluginHost::ISystemInfo::network_type SystemInfo::NetworkType() const
        {
            return (_ipAddress.empty() == true ? PluginHost::ISystemInfo::UNKNOWN : (Core::NodeId::IsIPV6Enabled() ? PluginHost::ISystemInfo::IPV6 : PluginHost::ISystemInfo::IPV4));
        }

        /* virtual */ void SystemInfo::PublicIPAddress(const string& ip)
        {
            _adminLock.Lock();

            _ipAddress = ip;

            _adminLock.Unlock();

            SYSLOG(PluginHost::Startup, (_T("SET: IP: %s"), ip.c_str()));

            Update();
        }

        // Location Information
        /* virtual */ string SystemInfo::TimeZone() const
        {
            return (_timeZone);
        }

        /* virtual */ string SystemInfo::Country() const
        {
            return (_country);
        }

        /* virtual */ string SystemInfo::Region() const
        {
            return (_region);
        }

        /* virtual */ string SystemInfo::City() const
        {
            return (_city);
        }

        /* virtual */ void SystemInfo::Location(const string& timeZone, const string& country, const string& region, const string& city)
        {
            _adminLock.Lock();

            _timeZone = timeZone;
            _country = country;
            _region = region;
            _city = city;

            _adminLock.Unlock();

            SYSLOG(PluginHost::Startup, (_T("SET: TimeZone: %s, Country: %s, Region: %s, City: %s"), 
                                          timeZone.c_str(), 
                                          country.c_str(), 
                                          region.c_str(), 
                                          city.c_str()));

            Update();
        }

        // Device Identifier
        /* virtual */ uint8 SystemInfo::Identifier(const uint8 length, uint8 buffer[]) const
        {
            uint8 result = 0;

            _adminLock.Lock();
            if (_identifier != nullptr) {
                result = _identifier[0];
                ::memcpy(buffer, &(_identifier[1]), (result > length ? length : result));
            }
            _adminLock.Unlock();

            return (result);
        }

        /* virtual */ void SystemInfo::Identifier(const uint8 length, const uint8 buffer[])
        {
            _adminLock.Lock();

            if (_identifier != nullptr) {
                delete _identifier;
            }

            _identifier = SetIdentifier(length, buffer);

            _adminLock.Unlock();

            SYSLOG(PluginHost::Startup, (_T("SET: Identifier: %s"),
                                          Core::SystemInfo::Instance().Id(_identifier, ~0).c_str()));

            Update();
        }

        /* virtual */ uint64 SystemInfo::TimeSync() const
        {
            return (_timeSync);
        }

        /* virtual */ void SystemInfo::TimeSync(const uint64 ticks)
        {
            _timeSync = ticks;

            SYSLOG(PluginHost::Startup, (_T("SET: Time: %s"),
                                          Core::Time(ticks).ToRFC1123(false).c_str()));

            Update();
        }

        // Hardware status
        /* virtual */ PluginHost::ISystemInfo::system_state SystemInfo::SystemState () const 
        {
            return (_systemState);
        }

        /* virtual */ PluginHost::ISystemInfo::system_state SystemInfo::SystemState (const PluginHost::ISystemInfo::system_state newState)
        {
            _systemState = newState;

            SYSLOG(PluginHost::Startup, (_T("SET: SystemState: %d"), newState));

            Update();

            return (_systemState);
        }
 
    }
}  // namespace PluginHost
