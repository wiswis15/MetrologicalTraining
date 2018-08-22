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
        , _service(nullptr)
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
        string result;
        Config config;
        config.FromString(service->ConfigLine());
        string version = service->Version();

        if (LocationService::IsSupported(config.Source.Value()) == Core::ERROR_NONE) {
            _skipURL = static_cast<uint16_t>(service->WebPrefix().length());
            _source = config.Source.Value();
            _service = service;

            _sink.Initialize(service, config.Source.Value(), config.Interval.Value(), config.Retries.Value());
        }
        else {
            result = _T("URL for retrieving location is incorrect !!!");
        }

        // On success return empty, to indicate there is no error text.
        return (result);
    }

    /* virtual */ void LocationSync::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT (_service == service);

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

            PluginHost::ISubSystem*  subSystem = _service->SubSystems();

            ASSERT(subSystem == nullptr);

            const PluginHost::ISubSystem::IInternet* internet(subSystem->Get<PluginHost::ISubSystem::IInternet>());
            const PluginHost::ISubSystem::ILocation* location(subSystem->Get<PluginHost::ISubSystem::ILocation>());

            response->PublicIp = internet->PublicIPAddress();
            response->TimeZone = location->TimeZone();
            response->Region = location->Region();
            response->Country = location->Country();
            response->City = location->City();

            result->ContentType = Web::MIMETypes::MIME_JSON;
            result->Body(Core::proxy_cast<Web::IBody>(response));
        }
        else if (request.Verb == Web::Request::HTTP_POST) {

            if (index.Next()) {
                if ((index.Current() == "Sync") && (_source.empty() == false)) {
                    uint32_t error = _sink.Probe(_source, 1, 1);

                    if (error != Core::ERROR_NONE) {
                        result->ErrorCode = Web::STATUS_INTERNAL_SERVER_ERROR;
                        result->Message = _T("Probe failed with error code: ") + Core::NumberType<uint32_t>(error).Text();
                    }
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
        PluginHost::ISubSystem* subSystem = _service->SubSystems();

        ASSERT(subSystem != nullptr);

        if (subSystem != nullptr) {

            subSystem->Set(PluginHost::ISubSystem::INTERNET, _sink.Network());
            subSystem->Set(PluginHost::ISubSystem::LOCATION, _sink.Location());
            subSystem->Release();

            if ( (_sink.Location() != nullptr) && (_sink.Location()->TimeZone().empty() == false) ) {
                Core::SystemInfo::SetEnvironment(_T("TZ"), _sink.Location()->TimeZone());
            }
        }
    }

} // namespace Plugin
} // namespace WPEFramework
