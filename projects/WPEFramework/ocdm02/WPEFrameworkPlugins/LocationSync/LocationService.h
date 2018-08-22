#ifndef LOCATIONSYNC_LOCATIONSERVICE_H
#define LOCATIONSYNC_LOCATIONSERVICE_H

#include "Module.h"

namespace WPEFramework {

namespace Plugin {

    struct IGeography;

    class EXTERNAL LocationService
        : public PluginHost::ISubSystem::ILocation,
          public PluginHost::ISubSystem::IInternet,
          public Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request, Core::ProxyPoolType<Web::Response> &> {

    private:
        enum state {
            IDLE,
            ACTIVE,
            IPV6_INPROGRESS,
            IPV4_INPROGRESS,
            LOADED,
            FAILED
        };

        class Job : public Core::IDispatchType<void> {
        private:
            Job() = delete;
            Job(const Job&) = delete;
            Job& operator=(const Job&) = delete;

        public:
            Job(LocationService* parent)
                : _parent(*parent)
            {
                ASSERT(parent != nullptr);
            }
            ~Job()
            {
            }

        public:
            virtual void Dispatch() override
            {
                _parent.Dispatch();
            }

        private:
            LocationService& _parent;
        };

    private:
        LocationService() = delete;
        LocationService(const LocationService&) = delete;
        LocationService& operator=(const LocationService&) = delete;

        typedef Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request, Core::ProxyPoolType<Web::Response>&> BaseClass;

    public:
        LocationService(Core::IDispatchType<void>* update);
        virtual ~LocationService();

    public:
        BEGIN_INTERFACE_MAP(LocationService)
        INTERFACE_ENTRY(PluginHost::ISubSystem::ILocation)
        INTERFACE_ENTRY(PluginHost::ISubSystem::IInternet)
        END_INTERFACE_MAP

        // Retry TimeSpan is in Seconds.

        static uint32_t IsSupported(const string& remoteNode);
        uint32_t Probe(const string& remoteNode, const uint32_t retries, const uint32_t retryTimeSpan);
        void Stop();

      /*
       * ------------------------------------------------------------------------------------------------------------
       * ISubSystem::INetwork methods
       * ------------------------------------------------------------------------------------------------------------
       */
        virtual string PublicIPAddress() const
        {
            return (_publicIPAddress);
        }
        virtual network_type NetworkType() const
        {
            return (_publicIPAddress.empty() == true ? PluginHost::ISubSystem::IInternet::UNKNOWN : (Core::NodeId::IsIPV6Enabled() ? PluginHost::ISubSystem::IInternet::IPV6 : PluginHost::ISubSystem::IInternet::IPV4));
        }
      /*
       * ------------------------------------------------------------------------------------------------------------
       * ISubSystem::ILocation methods
       * ------------------------------------------------------------------------------------------------------------
       */
        virtual string TimeZone() const
        {
            return (_timeZone);
        }
        virtual string Country() const
        {
            return (_country);
        }
        virtual string Region() const
        {
            return (_region);
        }
        virtual string City() const
        {
            return (_city);
        }

    private:
        // Notification of a Partial Request received, time to attach a body..
        virtual void LinkBody(Core::ProxyType<Web::Response>& element) override;
        virtual void Received(Core::ProxyType<Web::Response>& element) override;
        virtual void Send(const Core::ProxyType<Web::Request>& element) override;

        // Signal a state change, Opened, Closed or Accepted
        virtual void StateChange() override;

        void Dispatch();


    private:
        Core::CriticalSection _adminLock;
        state _state;
        string _remoteId;
        Core::NodeId _sourceNode;
        uint32_t _tryInterval;
        uint32_t _retries;
        Core::IDispatch* _callback;
        string _publicIPAddress;
        string _timeZone;
        string _country;
        string _region;
        string _city;
		Core::ProxyType<IGeography> _infoCarrier;

        Core::ProxyType<Web::Request> _request;
        Core::ProxyType<Web::IBody > _response;
        Core::ProxyType<Core::IDispatch > _activity;
    };
}
} // namespace WPEFramework:Plugin

#endif // LOCATIONSYNC_LOCATIONSERVICE_H
