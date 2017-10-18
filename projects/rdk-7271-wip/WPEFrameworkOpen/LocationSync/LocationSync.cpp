#include "LocationSync.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(LocationSync, 1, 0);

    static Core::ProxyPoolType<Web::Response> responseFactory(4);
    static Core::ProxyPoolType<Web::JSONBodyType<LocationSync::Data> > jsonResponseFactory(4);

	#ifdef __WIN32__ 
	#pragma warning( disable : 4355 )
	#endif
    LocationSync::LocationSync()
        : _skipURL(0)
        , _source()
        , _sink(this)
    {
    }
	#ifdef __WIN32__ 
	#pragma warning( default : 4355 )
	#endif

    /* virtual */ LocationSync::~LocationSync()
    {
    }

    /* virtual */ const string LocationSync::Initialize(PluginHost::IShell* service)
    {
        Config config;
        config.FromString(service->ConfigLine());
        string version = service->Version();
        _skipURL = static_cast<uint16>(service->WebPrefix().length());
        _source = config.Source.Value();
        _sink.Initialize(service, config.System.Value());
        _sink.Probe(config.Source.Value(), config.Retries.Value(), config.Interval.Value());

        // On success return empty, to indicate there is no error text.
        return _T("");
    }

    /* virtual */ void LocationSync::Deinitialize(PluginHost::IShell* service)
    {
        _sink.Deinitialize();
    }

    /* virtual */ string LocationSync::Information() const
    {
        // No additional info to report.
        return (string());
    }

    /* virtual */ void LocationSync::Inbound(Web::Request& /* request */)
    {
    }

    /* virtual */ Core::ProxyType<Web::Response>
    LocationSync::Process(const Web::Request& request)
    {
        Core::ProxyType<Web::Response> result(PluginHost::Factories::Instance().Response());
        Core::TextSegmentIterator index(
            Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL),
            false,
            '/');

        // By default, we assume everything works..
        result->ErrorCode = Web::STATUS_OK;
        result->Message = "OK";

        if (request.Verb == Web::Request::HTTP_GET) {
            Core::ProxyType<Web::JSONBodyType<Data> > response(jsonResponseFactory.Element());

            response->PublicIp = _sink.PublicIPAddress();
            response->TimeZone = _sink.TimeZone();
            response->Region = _sink.Region();
            response->Country = _sink.Country();
            response->City = _sink.City();

            result->ContentType = Web::MIMETypes::MIME_JSON;
            result->Body(Core::proxy_cast<Web::IBody>(response));
        }
        else if (request.Verb == Web::Request::HTTP_POST) {

            if (index.Next()) {
                if ((index.Current() == "Sync") && (_source.empty() == false)) {
                    _sink.Probe(_source, 1, 1);
                }
            }
        }
        else {
            result->ErrorCode = Web::STATUS_BAD_REQUEST;
            result->Message = _T("Unsupported request for the [LocationSync] service.");
        }

        return result;
    }

    void LocationSync::SyncedLocation()
    {
		const string timeZone(_sink.TimeZone());

		if (timeZone.empty() == false)
		{
			Core::SystemInfo::SetEnvironment(_T("TZ"), timeZone);
		}
    }

} // namespace Plugin
} // namespace WPEFramework
