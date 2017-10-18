#ifndef LOCATIONSYNC_LOCATIONSERVICE_H
#define LOCATIONSYNC_LOCATIONSERVICE_H

#include "Module.h"

namespace WPEFramework {

namespace Plugin {

    class EXTERNAL LocationService
        : public Web::WebLinkType<Core::SocketStream, Web::Response, Web::Request, Core::ProxyPoolType<Web::Response>&> {
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

        class Response : public Core::JSON::Container {
        private:
            Response(const Response&) = delete;
            Response& operator=(const Response&) = delete;

            class Geography : public Core::JSON::Container {

            private:
                Geography& operator=(const Geography&) = delete;

            public:
                Geography()
                    : Core::JSON::Container()
                    , Country()
                    , City()
                    , Region()
                    , TimeZone()
                    , LL()
                {
                    Add(_T("country"), &Country);
                    Add(_T("city"), &City);
                    Add(_T("region"), &Region);
                    Add(_T("tz"), &TimeZone);
                    Add(_T("ll"), &LL);
                }
                Geography(const Geography& copy)
                    : Core::JSON::Container()
                    , Country(copy.Country)
                    , City(copy.City)
                    , Region(copy.Region)
                    , TimeZone(copy.TimeZone)
                    , LL(copy.LL)
                {
                    Add(_T("country"), &Country);
                    Add(_T("city"), &City);
                    Add(_T("region"), &Region);
                    Add(_T("tz"), &TimeZone);
                    Add(_T("ll"), &LL);
                }
                virtual ~Geography()
                {
                }

            public:
                Core::JSON::String Country;
                Core::JSON::String City;
                Core::JSON::String Region;
                Core::JSON::String TimeZone;
                Core::JSON::String LL;
            };

        public:
            Response()
                : Core::JSON::Container()
                , IP()
                , Geo()
            {
                Add(_T("ip"), &IP);
                Add(_T("ping"), &Ping);
                Add(_T("geo"), &Geo);
            }
            virtual ~Response()
            {
            }

        public:
            Core::JSON::String IP;
            Core::JSON::Boolean Ping;
            Geography Geo;
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
        // Retry TimeSpan is in Seconds.
        uint32 Probe(const string& remoteNode, const uint32 retries, const uint32 retryTimeSpan);
        void Stop();

        inline const string& PublicIPAddress() const
        {
            return (_publicIPAddress);
        }
        inline const string& TimeZone() const
        {
            return (_timeZone);
        }
        inline const string& Country() const
        {
            return (_country);
        }
        inline const string& Region() const
        {
            return (_region);
        }
        inline const string& City() const
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
        uint32 _tryInterval;
        uint32 _retries;
        Core::IDispatchType<void>* _callback;
        string _publicIPAddress;
        string _timeZone;
        string _country;
        string _region;
        string _city;

        Core::ProxyType<Web::Request> _request;
        Core::ProxyType<Web::JSONBodyType<Response> > _response;
        Core::ProxyType<Core::IDispatchType<void> > _activity;
    };
}
} // namespace WPEFramework:Plugin

#endif // LOCATIONSYNC_LOCATIONSERVICE_H
