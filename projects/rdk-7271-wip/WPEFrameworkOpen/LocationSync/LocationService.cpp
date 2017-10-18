#include "LocationService.h"

namespace WPEFramework {
namespace Plugin {

    static Core::ProxyPoolType<Web::Response> g_Factory(1);

    static Core::NodeId FindLocalIPV6()
    {
        Core::NodeId result;
        Core::AdapterIterator interfaces;

        while ((result.IsValid() == false) && (interfaces.Next() == true)) {

            Core::IPV6AddressIterator index(interfaces.IPV6Addresses());

            TRACE_L1("Checking interface: %s", interfaces.Name().c_str());

            while ((result.IsValid() == false) && (index.Next() == true)) {
                if (index.IsValid() && index.IsUnicast()) {
                    TRACE_L1("Found IPV6 address: %s", index.Address().HostAddress().c_str());
                    result = index.Address();
                }
            }
        }

        return (result);
    }

	#ifdef __WIN32__ 
	#pragma warning( disable : 4355 )
	#endif
    LocationService::LocationService(Core::IDispatchType<void>* callback)
        : BaseClass(1, g_Factory, false, Core::NodeId(), Core::NodeId(), 256, 1024)
        , _adminLock()
        , _state(IDLE)
        , _remoteId()
        , _sourceNode()
        , _tryInterval(0)
        , _callback(callback)
        , _publicIPAddress()
        , _timeZone()
        , _country()
        , _region()
        , _city()
        , _request(Core::ProxyType<Web::Request>::Create())
        , _response(Core::ProxyType<Web::JSONBodyType<Response> >::Create())
        , _activity(Core::ProxyType<Job>::Create(this))
    {
        _request->Verb = Web::Request::HTTP_GET;
        _request->Path = _T("/");
        _request->Query = _T("maf=true");
    }
	#ifdef __WIN32__ 
	#pragma warning( default : 4355 )
	#endif

	/* virtual */ LocationService::~LocationService()
    {

        Stop();

        Close(Core::infinite);
    }

    uint32 LocationService::Probe(const string& remoteNode, const uint32 retries, const uint32 retryTimeSpan)
    {

        uint32 result = Core::ERROR_INPROGRESS;

        _adminLock.Lock();

        if ((_state == IDLE) || (_state == FAILED) || (_state == LOADED)) {
            _state = ACTIVE;
            // it runs till zero, so subtract by definition 1 :-)
            _retries = (retries - 1);
            _remoteId = remoteNode;
            _tryInterval = retryTimeSpan * 1000; // Move from seconds to mS.
            _request->Host = remoteNode;

            PluginHost::WorkerPool::Instance().Submit(_activity);

            result = Core::ERROR_NONE;
        }

        _adminLock.Unlock();

        return (result);
    }

    void LocationService::Stop()
    {

        _adminLock.Lock();

        PluginHost::WorkerPool::Instance().Revoke(_activity);

        if ((_state != IDLE) && (_state != FAILED) && (_state != LOADED)) {

            if (IsClosed() == false) {
                Close(0);
            }

            _state = FAILED;
        }

        _adminLock.Unlock();
    }

    // Methods to extract and insert data into the socket buffers
    /* virtual */ void LocationService::LinkBody(Core::ProxyType<Web::Response>& element)
    {
        if (element->ErrorCode == Web::STATUS_OK) {
            element->Body<Web::JSONBodyType<Response> >(_response);
        }
    }

    /* virtual */ void LocationService::Received(Core::ProxyType<Web::Response>& element)
    {

        if (element->HasBody() == true) {

            ASSERT(element->Body<Web::JSONBodyType<Response> >() == _response);

            _adminLock.Lock();

            _timeZone = _response->Geo.TimeZone.Value();
            _country = _response->Geo.Country.Value();
            _region = _response->Geo.Region.Value();
            _city = _response->Geo.City.Value();

            if (_state == IPV6_INPROGRESS) {

                // For now the source IPV6 is not returned but as IPV6 is not NAT'ed our IF Address should be
                // the outside IP address as well.
                Core::NodeId localId(FindLocalIPV6());

                ASSERT(localId.IsValid() == true);

                _publicIPAddress = localId.HostAddress();
            }
            else {
                _publicIPAddress = _response->IP.Value();
            }
            _state = LOADED;

            ASSERT(!_publicIPAddress.empty());

            Core::NodeId node(_publicIPAddress.c_str(), Core::NodeId::TYPE_UNSPECIFIED);

            if (node.IsValid() == true) {

                if (node.Type() == Core::NodeId::TYPE_IPV4) {
                    Core::NodeId::ClearIPV6Enabled();
                }

				TRACE_L1("Network connectivity established on %s. ip: %s, tz: %s, country: %s",
					node.Type() == Core::NodeId::TYPE_IPV4 ? _T("IPv4") : _T("IP6"),
					_publicIPAddress.c_str(),
					_timeZone.c_str(),
					_country.c_str());

                TRACE(Trace::Information, (_T("LocationSync: Network connectivity established. Type: %s, on %s"),
                    (node.Type() == Core::NodeId::TYPE_IPV6 ? _T("IPv6") : _T("IPv4")), node.HostAddress().c_str()));
                _callback->Dispatch();
            }

            _adminLock.Unlock();
        }
        else {
            TRACE_L1("Got a response but had an empty body. %d", __LINE__);
        }

        // Finish the cycle..
        PluginHost::WorkerPool::Instance().Submit(_activity);
    }

    /* virtual */ void LocationService::Send(const Core::ProxyType<Web::Request>& element)
    {

        // Not much to do, just so we know we are done...
        ASSERT(element == _request);
    }

    // Signal a state change, Opened, Closed or Accepted
    /* virtual */ void LocationService::StateChange()
    {

        if (Link().IsOpen() == true) {

            // Send out a trigger to send the request
            Submit(_request);
        }
        else if (Link().HasError() == true) {
            Close(0);
            PluginHost::WorkerPool::Instance().Submit(_activity);
        }
    }

    // The network might be down, keep on trying until we have connectivity.
    // We start using IPV6, preferred network...
    void LocationService::Dispatch()
    {

        uint32 result = Core::infinite;

        if ((IsClosed() == false) || (Close(100) != Core::ERROR_NONE)) {

            result = 500; // ms...Check again..
        }
        else {

            _adminLock.Lock();

            if (_state == IPV4_INPROGRESS) {
                _state = (_retries-- == 0 ? FAILED : ACTIVE);
            }

            if ((_state != LOADED) && (_state != FAILED)) {

                Core::NodeId remote(_remoteId.c_str(), ((_state == ACTIVE) && (Core::NodeId::IsIPV6Enabled()) ? Core::NodeId::TYPE_IPV6 : Core::NodeId::TYPE_IPV4));

                if (remote.IsValid() == false) {

                    TRACE_L1("DNS resolving failed. Sleep for %d mS for attempt %d", _tryInterval, _retries);

                    // Name resolving does not even work. Retry this after a few seconds, if we still can..
                    if (_retries-- == 0)
                        _state = FAILED;
                    else
                        result = _tryInterval;
                }
                else {
                    Link().LocalNode(remote.AnyInterface());
                    Link().RemoteNode(remote);

                    _state = (remote.Type() == Core::NodeId::TYPE_IPV6 ? IPV6_INPROGRESS : IPV4_INPROGRESS);

                    uint32 status = Open(0);

                    if ((status == Core::ERROR_NONE) || (status == Core::ERROR_INPROGRESS)) {

                        TRACE_L1("Sending out a network package on %s. Attempt: %d", (remote.Type() == Core::NodeId::TYPE_IPV6 ? _T("IPv6") : _T("IPv4")), _retries);

                        // We need to get a response in the given time..
                        result = _tryInterval;
                    }
                    else {
                        TRACE_L1("Failed on network %s. Reschedule for the next attempt: %d", (remote.Type() == Core::NodeId::TYPE_IPV6 ? _T("IPv6") : _T("IPv4")), _retries);

                        // Seems we could not open this connection, move on to the next attempt.
                        Close(0);
                        result = 100;
                    }
                }
            }
            _adminLock.Unlock();
        }

        if (_state == FAILED) {
            Core::NodeId::ClearIPV6Enabled();

            TRACE(Trace::Information, (_T("LocationSync: Network connectivity could *NOT* be established. Falling back to IPv4. %d"), __LINE__));
            _callback->Dispatch();
        }

        // See if we need rescheduling
        if (result != Core::infinite) {
            Core::Time timestamp(Core::Time::Now());
            timestamp.Add(result);
            PluginHost::WorkerPool::Instance().Schedule(timestamp, _activity);
        }
    }

} // namespace Plugin
} // namespace WPEFramework
