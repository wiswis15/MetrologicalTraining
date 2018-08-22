#include "TraceControl.h"
#include "TraceOutput.h"

namespace WPEFramework {

ENUM_CONVERSION_BEGIN(Plugin::TraceControl::state)

	{ Plugin::TraceControl::state::ENABLED,   _TXT("enabled")   },
	{ Plugin::TraceControl::state::DISABLED,  _TXT("disabled")  },
	{ Plugin::TraceControl::state::TRISTATED, _TXT("tristated") },

ENUM_CONVERSION_END(Plugin::TraceControl::state);

namespace Plugin {

    SERVICE_REGISTRATION(TraceControl, 1, 0);

	// Declare the local trace iterator exposing the remote interface.
	/* static */ TraceControl::Observer::Source::LocalIterator TraceControl::Observer::Source::_localIterator;
    static Core::ProxyPoolType<Web::JSONBodyType<TraceControl::Data> > jsonBodyDataFactory(4);

	/* static */ string TraceControl::Observer::Source::SourceName(RPC::IRemoteProcess* process)
	{
		string pathName; Core::SystemInfo::GetEnvironment(TRACE_CYCLIC_BUFFER_ENVIRONMENT, pathName);

		return (Core::Directory::Normalize(pathName) +
				TRACE_CYCLIC_BUFFER_PREFIX + '.' +
				Core::NumberType<uint32_t>(process == nullptr ? Core::ProcessInfo().Id() : process->Id()).Text());
	}

    /* virtual */ const string TraceControl::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
		ASSERT(_outputs.size() == 0);

        _service = service;
        _config.FromString(_service->ConfigLine());

        _skipURL = static_cast<uint8_t>(_service->WebPrefix().length());

        if (((service->Background() == false) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false)) || ((_config.Console.IsSet() == true) && (_config.Console.Value() == true))) {
			_outputs.push_back(new Plugin::TraceOutput(false));

        }
        if (((service->Background() == true) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false)) || ((_config.SysLog.IsSet() == true) && (_config.SysLog.Value() == true))) {
			_outputs.push_back(new Plugin::TraceOutput(true));

        }
        if (_config.Remote.IsSet() == true) {
            Core::NodeId logNode(_config.Remote.Binding.Value().c_str(), _config.Remote.Port.Value());

			_outputs.push_back(new Trace::TraceMedia(logNode));

        }

		_service->Register(&_observer);

		// Start observing..
		_observer.Run();

        // On succes return a name as a Callsign to be used in the URL, after the "service"prefix
        return (_T(""));
    }

    /* virtual */ void TraceControl::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT(service == _service);

		_service->Unregister(&_observer);

		// Stop observing..
		_observer.Pause();

		while (_outputs.size() != 0)
		{
			delete _outputs.front();

			_outputs.pop_front();
        }
    }

    /* virtual */ string TraceControl::Information() const
    {
        // No additional info to report.
        return (nullptr);
    }

    /* virtual */ void TraceControl::Inbound(Web::Request& /* request */)
    {
        // There are no requests coming in with a body that require a JSON body. So continue without action here.
    }

    // <GET> ../
    // <PUT> ../[on,off]
    // <PUT> ../<ModuleName>/[on,off]
    // <PUT> ../<ModuleName>/<CategoryName>/[on,off]
    /* virtual */ Core::ProxyType<Web::Response> TraceControl::Process(const Web::Request& request)
    {
        ASSERT(_skipURL <= request.Path.length());

        Core::ProxyType<Web::Response> result(PluginHost::Factories::Instance().Response());
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL), false, '/');

        // If there is an entry, the first one will alwys be a '/', skip this one..
        index.Next();

        // By default, we assume everything works..
        result->ErrorCode = Web::STATUS_OK;
        result->Message = "OK";

        if (request.Verb == Web::Request::HTTP_GET) {
            Core::ProxyType<Web::JSONBodyType<TraceControl::Data> > response(jsonBodyDataFactory.Element());
            // Nothing more required, just return the current status...
            response->Console = _config.Console;
            response->Remote = _config.Remote;

            Observer::ModuleIterator index(_observer.Modules());

            while (index.Next() == true) {
                string moduleName(Core::ToString(index.Module()));

				Observer::ModuleIterator::CategoryIterator categories(index.Categories());

				while (categories.Next())
				{
					string categoryName(Core::ToString(categories.Category()));

					response->Settings.Add(Data::Trace(moduleName, categoryName, categories.State()));
				}
            }

            result->Body(Core::proxy_cast<Web::IBody>(response));
            result->ContentType = Web::MIME_JSON;
        }
        else if ((request.Verb == Web::Request::HTTP_PUT) && (index.Next() == true)) {
            if ((index.Current() == _T("on")) || (index.Current() == _T("off"))) {
                // Done, no options, enable/disable all
                _observer.Set(
                    (index.Current() == _T("on")),
                    string(EMPTY_STRING),
					string(EMPTY_STRING));
            }
            else {
                std::string moduleName (Core::ToString(index.Current().Text()));

                if (index.Next() == true) {
                    if ((index.Current() == _T("on")) || (index.Current() == _T("off"))) {
                        // Done, only a modulename is set, enable/disable all
                        _observer.Set(
                            (index.Current() == _T("on")),
                            (moduleName.length() != 0 ? moduleName : std::string(EMPTY_STRING)),
							std::string(EMPTY_STRING));
                    }
                    else {
                        std::string categoryName (Core::ToString(index.Current().Text()));

                        if (index.Next() == true) {
                            if ((index.Current() == _T("on")) || (index.Current() == _T("off"))) {
                                // Done, only a modulename is set, enable/disable all
								_observer.Set(
                                    (index.Current() == _T("on")),
                                    (moduleName.length() != 0 ? moduleName : std::string(EMPTY_STRING)),
                                    (categoryName.length() != 0 ? categoryName : std::string(EMPTY_STRING)));
                            }
                            else {
                                result->ErrorCode = Web::STATUS_BAD_REQUEST;
                                result->Message = _T(" could not handle your request, last parameter should be [on,off].");
                            }
                        }
                        else {
                            result->ErrorCode = Web::STATUS_BAD_REQUEST;
                            result->Message = _T(" could not handle your request, last parameter should be the category.");
                        }
                    }
                }
                else {
                    result->ErrorCode = Web::STATUS_BAD_REQUEST;
                    result->Message = _T(" could not handle your request.");
                }
            }
        }
        else {
            result->ErrorCode = Web::STATUS_BAD_REQUEST;
            result->Message = _T(" could not handle your request.");
        }

		// Make sure all interface aquired for this action are released again otherwise
		// a process can not shut down if the plugin is deactivated due to an trace 
		// interface still referenced !!!!
		_observer.Relinquish();

        return (result);
    }


	void TraceControl::Dispatch(Observer::Source& information)
	{
		std::list<Trace::ITraceMedia*>::iterator index(_outputs.begin());
		InformationWrapper wrapper(information);

		while (index != _outputs.end())
		{
			(*index)->Output(information.FileName(), information.LineNumber(), information.ClassName(), &wrapper);
			index++;
		}
	}

}
}
